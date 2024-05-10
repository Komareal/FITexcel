#include "test.h"
using namespace std;

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

void setCellRange(const std::vector<std::string> &cells, const std::vector<std::string> &values, CSpreadsheet &spreadsheet) {
    for (int j = 0; j < cells.size(); j++) {
        spreadsheet.setCell(CPos(cells[j]), values[j]);
    }
}

void copyRectRange(const std::vector<std::pair<std::string, std::string> > &fromTo, int h, int w, CSpreadsheet &spreadsheet) {
    for (auto p: fromTo) {
        spreadsheet.copyRect(CPos(p.first), CPos(p.second), w, h);
    }
}

void saveLoad(CSpreadsheet &spreadsheet) {
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;

    oss.clear();
    oss.str("");
    assert(spreadsheet.save(oss));
    data = oss.str();
    std::cout << data << endl << endl;
    iss.clear();
    iss.str(data);
    assert(spreadsheet.load(iss));
}

// Function to add an edge to the graph
void addEdge(std::vector<std::vector<int> > &adj, int u, int v) {
    adj[u].push_back(v);
}


bool dfs(std::vector<std::vector<int> > &adj, std::vector<int> &color, int v, int parent) {
    if (color[v] == 1) {
        color[v] = 2;
        return true;
    }

    if (color[v] == 2) return false;

    color[v] = 1;
    for (int u: adj[v]) {
        if (dfs(adj, color, u, v)) return true;
        color[u] = 2;
    }
    return false;
}

bool hasCycle(std::vector<std::vector<int> > &adj, int from = 0, int to = -1) {
    int V = adj.size();
    if (to == -1)to = adj.size();

    std::vector<int> color(V, 0);
    for (int v = from; v < to; ++v) {
        if (color[v] == 0) {
            if (dfs(adj, color, v, -1))
                return true;
        }
    }
    return false;
}

bool generateRandomGraph(std::vector<std::vector<int> > &adj, int V, int E) {
    std::unordered_set<int> edgeSet;
    while (E > 0) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && edgeSet.find(u * V + v) == edgeSet.end()) {
            addEdge(adj, u, v);
            edgeSet.insert(u * V + v);
            if (rand() % 10 == 0) {
                addEdge(adj, v, u);
            }
            --E;
        }
    }
    return hasCycle(adj);
}

void generateTableWithCycles() {
    CSpreadsheet spreadsheet;
    int V = rand() % 100 + 10;
    int E = V + rand() % 10;
    std::vector<std::vector<int> > adj(V);
    bool hc = generateRandomGraph(adj, V, E);

    // make a labels to all nodes in a graph.
    std::unordered_map<int, std::string> nodeLabels;

    for (int j = 0; j < V; j++) {
        nodeLabels.insert({j, "A" + std::to_string(j)});
    }

    // Build a table.
    for (int j = 0; j < V; j++) {
        // generate expression.
        std::string expr = "=";
        for (auto node: adj[j]) {
            expr += "-" + nodeLabels[node];
        }
        expr += "-" + std::to_string(420);
        // Add to a table.
        spreadsheet.setCell(CPos(nodeLabels[j]), expr);
    }

    for (int j = 0; j < V; j++) {
        if (spreadsheet.getValue(CPos(nodeLabels[j])) == CValue()) {
            assert(hasCycle(adj, j,j+1) && adj[j].size()!=0);
        } else {
            auto value = spreadsheet.getValue(CPos(nodeLabels[j]));
            if (std::holds_alternative<double>(value)) {
                int v = std::stoi(std::to_string(std::abs(std::get<double>(value)))) % 420;
                assert(v==0);
            }
        }
    }
}

#define SIMPLE_TESTS // Simple tests - getVal, save & load - no file corruption.
#define CYCLIC_DEPS_TESTS // Cycle generation, if time > 2s -> exception
#define FILE_IO_TESTS // file corruption tests.
#include <future>
#include <chrono>

void runTests() {
    srand(time(nullptr));
    posTest();
    cellTest();
    basicTests();
    funcTest();
#ifdef SIMPLE_TESTS
    CSpreadsheet preTests;
    assert(preTests.setCell(CPos("d1"), "=12+10 + $E$1"));
    assert(valueMatch(preTests.getValue(CPos("d1")), CValue()));
    assert(preTests.setCell(CPos("e1"), "=-12-10"));
    assert(preTests.setCell(CPos("f1"), "=$d1 + E$1"));
    CValue res = preTests.getValue(CPos("d1"));
    assert(valueMatch(res, CValue(0.)));
    assert(valueMatch(preTests.getValue(CPos("f1")), CValue(-22.)));
    setCellRange({"d2", "d3", "d4", "d5", "g1", "h1", "h1"}, {"1", "2", "3", "4", "1", "=2", "=3", "=4"}, preTests);
    copyRectRange({{"g2", "f1"}, {"g3", "g2"}, {"g4", "f1"}, {"g5", "f1"}}, 1, 1, preTests);
    res = preTests.getValue(CPos("g2"));
    assert(valueMatch(res, CValue(-21.)));
    assert(valueMatch(preTests.getValue(CPos("g3")), CValue(-20.)));
    assert(valueMatch(preTests.getValue(CPos("g4")), CValue(-19.)));
    assert(valueMatch(preTests.getValue(CPos("g5")), CValue(-18.)));

    copyRectRange({{"h2", "g2"}, {"i2", "h2"}, {"h2", "h2"}}, 4, 1, preTests);

    assert(valueMatch(preTests.getValue(CPos("h2")), CValue(2.)));
    assert(valueMatch(preTests.getValue(CPos("h3")), CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h4")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("h5")), CValue(5.)));

    assert(valueMatch(preTests.getValue(CPos("i2")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("i3")), CValue(5.)));
    assert(valueMatch(preTests.getValue(CPos("i4")), CValue(6.)));
    assert(valueMatch(preTests.getValue(CPos("i5")), CValue(7.)));

    saveLoad(preTests);

    assert(valueMatch(preTests.getValue(CPos("h2")), CValue(2.)));
    assert(valueMatch(preTests.getValue(CPos("h3")), CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h4")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("h5")), CValue(5.)));

    assert(valueMatch(preTests.getValue(CPos("i2")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("i3")), CValue(5.)));
    assert(valueMatch(preTests.getValue(CPos("i4")), CValue(6.)));
    assert(valueMatch(preTests.getValue(CPos("i5")), CValue(7.)));

    assert(valueMatch(preTests.getValue(CPos("f1")), CValue(-22.)));
    res = preTests.getValue(CPos("g2"));

    preTests.copyRect(CPos("h3"), CPos("g2"), 4, 4);
    res = preTests.getValue(CPos("h3"));
    assert(valueMatch(res, CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("i6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("h6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k3")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k4")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k5")), CValue()));

    saveLoad(preTests);

    assert(valueMatch(preTests.getValue(CPos("h2")), CValue(2.)));
    res = preTests.getValue(CPos("h3"));
    assert(valueMatch(res, CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h4")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("h5")), CValue(5.)));

    assert(valueMatch(preTests.getValue(CPos("i2")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("i3")), CValue(5.)));
    assert(valueMatch(preTests.getValue(CPos("i4")), CValue(6.)));
    assert(valueMatch(preTests.getValue(CPos("i5")), CValue(7.)));

    assert(valueMatch(preTests.getValue(CPos("f1")), CValue(-22.)));

    assert(valueMatch(preTests.getValue(CPos("h6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("i6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("h6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k3")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k4")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k5")), CValue()));

    preTests.setCell(CPos("d6"), "0");

    assert(valueMatch(preTests.getValue(CPos("h6")), CValue(1.)));
    assert(valueMatch(preTests.getValue(CPos("i6")), CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h6")), CValue(1.)));
    assert(valueMatch(preTests.getValue(CPos("k6")), CValue()));

    preTests.setCell(CPos("d10"), "=$d11+$E$10");
    assert(valueMatch(preTests.getValue(CPos("d10")), CValue()));

    preTests.setCell(CPos("d11"), "=1");
    preTests.setCell(CPos("e10"), "=10");
    assert(valueMatch(preTests.getValue(CPos("d10")), CValue(11.)));

    copyRectRange({{"i2", "d10"}, {"i3", "i2"}, {"i4", "d10"}, {"i5", "d10"}}, 1, 1, preTests);

    assert(valueMatch(preTests.getValue(CPos("i2")), CValue(12.)));
    assert(valueMatch(preTests.getValue(CPos("i3")), CValue(13.)));
    assert(valueMatch(preTests.getValue(CPos("i4")), CValue(14.)));
    assert(valueMatch(preTests.getValue(CPos("i5")), CValue(10.)));

    saveLoad(preTests);

    assert(valueMatch(preTests.getValue(CPos("d10")), CValue(11.)));

    assert(valueMatch(preTests.getValue(CPos("i2")), CValue(12.)));
    assert(valueMatch(preTests.getValue(CPos("i3")), CValue(13.)));
    assert(valueMatch(preTests.getValue(CPos("i4")), CValue(14.)));
    assert(valueMatch(preTests.getValue(CPos("i5")), CValue(10.)));

    assert(valueMatch(preTests.getValue(CPos("h2")), CValue(2.)));
    assert(valueMatch(preTests.getValue(CPos("h3")), CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h4")), CValue(4.)));
    assert(valueMatch(preTests.getValue(CPos("h5")), CValue(5.)));

    assert(valueMatch(preTests.getValue(CPos("f1")), CValue(-22.)));

    assert(valueMatch(preTests.getValue(CPos("h6")), CValue(1.)));
    assert(valueMatch(preTests.getValue(CPos("i6")), CValue(3.)));
    assert(valueMatch(preTests.getValue(CPos("h6")), CValue(1.)));
    assert(valueMatch(preTests.getValue(CPos("k6")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k3")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k4")), CValue()));
    assert(valueMatch(preTests.getValue(CPos("k5")), CValue()));
    std::cout << "SIMPLE_TESTS PASSED\n";
#endif

#ifdef CYCLIC_DEPS_TESTS
    int TC = 150;
    while (TC--) {
        std::future<void> future = std::async(std::launch::async, generateTableWithCycles);
        if (future.wait_for(std::chrono::seconds(2)) == std::future_status::timeout) {
            throw std::runtime_error("Cycle detection went wrong!");
        }
    }

    std::cout << "CYCLIC_DEPS_TESTS PASSED\n";
#endif

#ifdef FILE_IO_TESTS

    CSpreadsheet fileIo;
    setCellRange({"A1", "B1", "B3", "A8", "ZZZ8", "AAA69", "ZZZ1", "AHOJ1", "Never1", "Gonna1", "Give1", "You1", "Up1"}, {"10", "=10+50", "A1+B1", "B3+A8", "5", "10", "Lez", "1000", "Never", "Gonna", "Let", "U", "Down"}, fileIo);

    std::ostringstream oss;
    std::istringstream iss;
    std::string data;

    saveLoad(fileIo);

    oss.clear();
    oss.str("");
    assert(fileIo.save(oss));
    data = oss.str();
    // swap 2 characters.
    for (int j = 0; j < data.length(); j++) {
        for (int i = j + 1; i < data.length(); i++) {
            std::string copy = data;
            std::swap(copy[j], copy[i]);
            iss.clear();
            iss.str(copy);
            bool resb = fileIo.load(iss) == (copy == data);
            if (!resb) {
                cout << "failed:" << endl << copy << endl << endl << data;
            }
            assert(resb);
        }
    }

    // erase 1 char randomly
    for (int j = 0; j < 50; j++) {
        oss.clear();
        oss.str("");
        assert(fileIo.save(oss));
        data = oss.str();
        data.erase(rand() % data.length(), 1);
        iss.clear();
        iss.str(data);
        assert(!fileIo.load(iss));
    }


    CSpreadsheet copied = fileIo;
    setCellRange({"A1", "B1", "B3", "A8", "ZZZ8", "AAA69", "ZZZ1", "AHOJ1", "Never1", "Gonna1", "Give1", "You1", "Up1"}, {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"}, fileIo);

    CSpreadsheet toLoad;
    oss.clear();
    oss.str("");
    assert(copied.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(toLoad.load(iss));

    assert(valueMatch(toLoad.getValue(CPos("Never1")), CValue("Never")));
    assert(valueMatch(fileIo.getValue(CPos("A1")), CValue(0.)));

    std::cout << "FILE_IO_TESTS PASSED\n";
#endif
}

void cellTest() {
    CSpreadsheet s;
    assert(!s.setCell(CPos("b2"), "=$0 * 2"));
    assert(!s.setCell(CPos("v2"), "=$2c"));
    s.setCell(CPos("b1"), "=10 * 2");
    s.setCell(CPos("A4"), "TEST");
    s.setCell(CPos("A3"), "=A4");
    s.setCell(CPos("PROG128"), "=(5*2+2)/2");
    s.setCell(CPos("PROG129"), "=((5*2)^2)/2");
    s.setCell(CPos("PROG1"), "=PROG129 * 2");
    s.setCell(CPos("b2"), "=B1 = 20 ");
    s.setCell(CPos("b3"), "=B1 >= 20 ");
    s.setCell(CPos("b4"), "=B1 <= 20 ");
    s.setCell(CPos("b5"), "=B1 > 20 ");
    s.setCell(CPos("b6"), "=B1 < 20 ");
    s.setCell(CPos("c1"), "=c2 ");
    s.setCell(CPos("c2"), "=c1 ");
    saveLoad(s);
    assert(valueMatch ( s . getValue ( CPos ( "B1" ) ), CValue ( 20.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "B2" ) ), CValue ( 1.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "B3" ) ), CValue ( 1.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "B4" ) ), CValue ( 1.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "B5" ) ), CValue ( 0.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "B6" ) ), CValue ( 0.0 ) ));
    assert(valueMatch ( s . getValue ( CPos ( "c1" ) ), CValue ( ) ));
    assert(valueMatch ( s . getValue ( CPos ( "c25" ) ), CValue ( ) ));
    auto res = s.getValue(CPos("A4"));
    cout << get<string>(res) << endl;
    res = s.getValue(CPos("PROG128"));
    cout << get<double>(res) << endl;
    res = s.getValue(CPos("PROG129"));
    cout << get<double>(res) << endl;
    res = s.getValue(CPos("PROG129"));
    cout << get<double>(res) << endl;
    res = s.getValue(CPos("PROG1"));
    cout << get<double>(res) << endl;

    CSpreadsheet l, m;
    l.setCell(CPos("A4"), "TEST");
    l.setCell(CPos("A2"), "dTEST");
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    oss.clear();
    oss.str("");
    assert(l . save ( oss ));
    data = oss.str();
    std::cout << data << endl << endl;
    iss.clear();
    iss.str(data);
    assert(m . load ( iss ));
    cout << ("efg"s > "ABC"s) << endl;
    CSpreadsheet x;
    assert(x.setCell(CPos("A0"), "ABC"));
    assert(x.setCell(CPos("A1"), "efg"));
    assert(x.setCell(CPos("A2"), "efg"));
    assert(x.setCell(CPos("B0"), "10"));
    assert(x.setCell(CPos("B1"), "20"));
    assert(x.setCell(CPos("B2"), "20"));
    assert(x.setCell(CPos("C0"), "= a0 + a1"));
    assert(x.setCell(CPos("C1"), "= b0 + b1"));
    assert(x.setCell(CPos("C2"), "= a0 + ((a1 + b0) + b1) "));
    assert(x.setCell(CPos("D0"), "= 10 / 0 "));
    assert(x.setCell(CPos("E1"), "= A0 - A1 "));
    assert(x.setCell(CPos("E2"), "= A0 * A1 "));
    assert(x.setCell(CPos("E3"), "= A0 ^ A1 "));

    assert(x.setCell(CPos("F0"), "= A0 > A1 "));
    assert(x.setCell(CPos("F1"), "= A0 < A1 "));
    assert(x.setCell(CPos("F2"), "= A0 <= A1 "));
    assert(x.setCell(CPos("F3"), "= A0 >= A1 "));
    assert(x.setCell(CPos("F4"), "= A0 = A1 "));
    assert(x.setCell(CPos("F5"), "= A0 <> A1 "));

    assert(x.setCell(CPos("g0"), "= B0 > B1 "));
    assert(x.setCell(CPos("g1"), "= B0 < B1 "));
    assert(x.setCell(CPos("g2"), "= B0 <= B1 "));
    assert(x.setCell(CPos("g3"), "= B0 >= B1 "));
    assert(x.setCell(CPos("g4"), "= B0 = B1 "));
    assert(x.setCell(CPos("g5"), "= B0 <> B1 "));

    assert(x.setCell(CPos("h0"), "= B2 > B1 "));
    assert(x.setCell(CPos("h1"), "= B2 < B1 "));
    assert(x.setCell(CPos("h2"), "= B2 <= B1 "));
    assert(x.setCell(CPos("h3"), "= B2 >= B1 "));
    assert(x.setCell(CPos("h4"), "= B2 = B1 "));
    assert(x.setCell(CPos("h5"), "= B2 <> B1 "));

    assert(x.setCell(CPos("i0"), "= A2 > A1 "));
    assert(x.setCell(CPos("i1"), "= A2 < A1 "));
    assert(x.setCell(CPos("i2"), "= A2 <= A1 "));
    assert(x.setCell(CPos("i3"), "= A2 >= A1 "));
    assert(x.setCell(CPos("i4"), "= A2 = A1 "));
    assert(x.setCell(CPos("i5"), "= A2 <> A1 "));


    assert(x.setCell(CPos("j0"), "= A2 > B1 "));
    assert(x.setCell(CPos("j1"), "= A2 < B1 "));
    assert(x.setCell(CPos("j2"), "= A2 <= B1 "));
    assert(x.setCell(CPos("j3"), "= A2 >= B1 "));
    assert(x.setCell(CPos("j4"), "= A2 = B1 "));
    assert(x.setCell(CPos("j5"), "= A2 <> B1 "));


    assert(valueMatch(x.getValue(CPos("d0")), CValue()));
    assert(valueMatch(x.getValue(CPos("e1")), CValue()));
    assert(valueMatch(x.getValue(CPos("e2")), CValue()));
    assert(valueMatch(x.getValue(CPos("e3")), CValue()));


    assert(valueMatch(x.getValue(CPos("f0")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("f1")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("f2")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("f3")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("f4")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("f5")), CValue(1.0)));

    assert(valueMatch(x.getValue(CPos("g0")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("g1")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("g2")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("g3")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("g4")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("g5")), CValue(1.0)));

    assert(valueMatch(x.getValue(CPos("h0")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("h1")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("h2")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("h3")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("h4")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("h5")), CValue(0.0)));


    assert(valueMatch(x.getValue(CPos("i0")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("i1")), CValue(0.0)));
    assert(valueMatch(x.getValue(CPos("i2")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("i3")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("i4")), CValue(1.0)));
    assert(valueMatch(x.getValue(CPos("i5")), CValue(0.0)));

    assert(valueMatch(x.getValue(CPos("j0")), CValue()));
    assert(valueMatch(x.getValue(CPos("j1")), CValue()));
    assert(valueMatch(x.getValue(CPos("j2")), CValue()));
    assert(valueMatch(x.getValue(CPos("j3")), CValue()));
    assert(valueMatch(x.getValue(CPos("j4")), CValue()));
    assert(valueMatch(x.getValue(CPos("j5")), CValue()));

    res = x.getValue(CPos("c0"));
    assert(valueMatch ( res, CValue ( "ABCefg" ) ));
    res = x.getValue(CPos("c1"));
    assert(valueMatch ( res, CValue ( 30.0 ) ));
    cout << std::to_string(10.0) << endl;
    res = x.getValue(CPos("c2"));
    assert(valueMatch ( res, CValue ( "ABCefg10.00000020.000000" ) ));

    cout << "cellTest OK" << std::endl;
}

void posTest() {
    // Basic operators
    CPos x("A0");
    CPos y("A0");
    assert(x == y);
    assert(x >= y);
    assert(x <= y);

    y = {"B0"};
    assert(x < y);
    assert(x <= y);
    assert(y > x);
    assert(y >= x);

    y = {"A1"};
    assert(x < y);
    y = {"PROG128"};
    assert(x < y);

    y = {"AA27"};

    y = x;
    assert(x == y);

    // Exceptions
    bool exc = false;
    try {
        y = {"0A"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);

    exc = false;
    try {
        y = {"ABC"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    exc = false;
    try {
        y = {"AZaz90"};
    } catch (...) {
        exc = true;
    }
    assert(!exc);

    exc = false;
    try {
        y = {"0123"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {" AB01"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {""};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {" AB 01"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {"AB01 "};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {"$A0"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {"A$0"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    try {
        y = {"$A$0"};
    } catch (const std::invalid_argument &) {
        exc = true;
    }
    assert(exc);
    exc = false;
    try {
        y = {"GKGWBYLWRXTLPP0"};
    } catch (const std::overflow_error &) {
        exc = true;
    }
    assert(exc);
    exc = false;
    try {
        y = {"A18446744073709551616"};
    } catch (const std::overflow_error &) {
        exc = true;
    }
    assert(exc);
    exc = false;
    try {
        y = {"GKGWBYLWRXTLPO0"};
    } catch (const std::overflow_error &) {
        exc = true;
    }
    assert(!exc);
    exc = false;
    try {
        y = {"A18446744073709551615"};
    } catch (const std::overflow_error &) {
        exc = true;
    }
    assert(!exc);

    // Moving
    y = {"B10", false};
    y = y.relativeMove(25, 33);
    assert(y == CPos("AA43"));

    y = {"$B10", false};
    y = y.relativeMove(25, 33);
    assert(y == CPos("$B43", false));

    y = {"B$10", false};
    y = y.relativeMove(25, 33);
    assert(y == CPos("AA$10", false));

    y = {"$B$10", false};
    y = y.relativeMove(25, 33);
    assert(y == CPos("$B$10", false));

    y = {"B10", false};
    y = y.relativeMove(-1, -10);
    assert(y == CPos("A0", false));
    y = {"Z0"};
    y = {"A0"};
    y = {"z0"};
    y = {"a0"};
    assert(CPos("AZ0") == CPos("az0"));

    std::cout << "posTest OK" << std::endl;
}

void funcTest() {
    CSpreadsheet s;
    setCellRange({"a1", "a2", "a3", "b2", "b3"}, {"-2", "100", "=-100", "=0", "2"}, s);
    setCellRange({"t1", "t2", "t3", "t4", "t5"}, {"=sum(A1:B3)", "=min(A1:B3)", "= max(A1:B3)", "= count(A1:B3)", "= countval(0, A1:B3)"}, s);

    saveLoad(s);

    assert(valueMatch(s.getValue(CPos("a1")), CValue(-2.)));
    assert(valueMatch(s.getValue(CPos("a2")), CValue(100.)));
    assert(valueMatch(s.getValue(CPos("a3")), CValue(-100.)));
    assert(valueMatch(s.getValue(CPos("b2")), CValue(0.)));
    assert(valueMatch(s.getValue(CPos("b3")), CValue(2.)));

    assert(valueMatch(s.getValue(CPos("t1")), CValue(0.)));
    assert(valueMatch(s.getValue(CPos("t2")), CValue(-100.)));
    assert(valueMatch(s.getValue(CPos("t3")), CValue(100.)));
    assert(valueMatch(s.getValue(CPos("t4")), CValue(5.)));
    assert(valueMatch(s.getValue(CPos("t5")), CValue(1.))); {
        CSpreadsheet xxx;
        assert(xxx.setCell(CPos("A0"), "1"));
        assert(xxx.setCell(CPos("A1"), "1"));
        assert(xxx.setCell(CPos("A2"), "1"));
        assert(xxx.setCell(CPos("A3"), "1"));
        assert(xxx.setCell(CPos("A4"), "1"));

        assert(xxx.setCell(CPos("B0"), "2"));
        assert(xxx.setCell(CPos("B1"), "2"));
        assert(xxx.setCell(CPos("B2"), "2"));
        assert(xxx.setCell(CPos("B3"), "2"));
        assert(xxx.setCell(CPos("B4"), "2"));

        assert(xxx.setCell(CPos("C0"), "3"));
        assert(xxx.setCell(CPos("C1"), "3"));
        assert(xxx.setCell(CPos("C2"), "3"));
        assert(xxx.setCell(CPos("C3"), "3"));
        assert(xxx.setCell(CPos("C4"), "3"));

        assert(xxx.setCell(CPos("D0"), "4"));
        assert(xxx.setCell(CPos("D1"), "4"));
        assert(xxx.setCell(CPos("D2"), "4"));
        assert(xxx.setCell(CPos("D3"), "4"));
        assert(xxx.setCell(CPos("D4"), "4"));

        assert(xxx.setCell(CPos("E0"), "5"));
        assert(xxx.setCell(CPos("E1"), "5"));
        assert(xxx.setCell(CPos("E2"), "5"));
        assert(xxx.setCell(CPos("E3"), "5"));
        assert(xxx.setCell(CPos("E4"), "5"));

        assert(xxx.setCell(CPos("F0"), "=sum(B2:D4)"));
        assert(!xxx.setCell(CPos("F0"), "=Sum(B2:D4)"));
        assert(!xxx.setCell(CPos("F0"), "=sum(B2)"));
        assert(!xxx.setCell(CPos("F0"), "=sum(B2:D4"));
        assert(!xxx.setCell(CPos("F0"), "=sum(B2:"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(xxx.setCell(CPos("D4"), "3"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(26.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(26.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(26.0)));
        assert(xxx.setCell(CPos("D4"), "4"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(xxx.setCell(CPos("F0"), "=sum(D4:B2)"));
        // assert(valueMatch(xxx.getValue(CPos("F0")), CValue(27.0)));
        assert(xxx.setCell(CPos("F0"), "=min(B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(2.0)));
        assert(xxx.setCell(CPos("F0"), "=max(B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(4.0)));
        assert(xxx.setCell(CPos("F0"), "=max(B2:D4)+min(B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(6.0)));
        assert(xxx.setCell(CPos("F0"), "=count(B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(9.0)));
        assert(xxx.setCell(CPos("F0"), "=countval(3, B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(3.0)));
        assert(xxx.setCell(CPos("F0"), "=countval(\"3\", B2:D4)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(0.0)));
        assert(!xxx.setCell(CPos("F0"), "=countval(D2:B2, B2:D4)"));
        assert(xxx.setCell(CPos("F0"), "=if(1, 2, 3)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(2.0)));
        assert(xxx.setCell(CPos("F0"), "=if(0, 2, 3)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(3.0)));
        assert(xxx.setCell(CPos("F0"), "=if(2, 2, 3)"));
        assert(valueMatch(xxx.getValue(CPos("F0")), CValue(2.0)));
        assert(!xxx.setCell(CPos("F0"), "=if(A2:A2, 2, 3)"));
        assert(!xxx.setCell(CPos("F0"), "=if(A2:A2, A2:A2, 3)"));
        assert(!xxx.setCell(CPos("F0"), "=if(A2:A2, A2:A2, A2:A2)"));
        assert(!xxx.setCell(CPos("F0"), "=if(0, A2:A2, A2:A2)"));
    }
}

#ifdef BASIC_TEST
void basicTests() {
    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    assert(x0 . setCell ( CPos ( "A1" ), "10" ));
    assert(x0 . setCell ( CPos ( "A2" ), "20.5" ));
    assert(x0 . setCell ( CPos ( "A3" ), "3e1" ));
    assert(x0 . setCell ( CPos ( "A4" ), "=40" ));
    assert(x0 . setCell ( CPos ( "A5" ), "=5e+1" ));
    assert(x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ));
    assert(x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ));
    assert(x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ));
    assert(x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ));
    assert(x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ));
    assert(x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ));
    assert(x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ));
    assert(x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ));
    CValue res = x0.getValue(CPos("B2"));
    assert(valueMatch ( res, CValue ( -110.25 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ));
    assert(x0 . setCell ( CPos ( "A1" ), "12" ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ));
    x1 = x0;
    assert(x0 . setCell ( CPos ( "A2" ), "100" ));
    assert(x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ));
    cout << get<double>(x0.getValue(CPos("A1"))) << " " << get<double>(x1.getValue(CPos("A1"))) << endl;
    cout << get<double>(x0.getValue(CPos("A2"))) << " " << get<double>(x1.getValue(CPos("A2"))) << endl;
    cout << get<double>(x0.getValue(CPos("A3"))) << " " << get<double>(x1.getValue(CPos("A3"))) << endl;
    cout << get<double>(x0.getValue(CPos("b1"))) << " " << get<double>(x1.getValue(CPos("b1"))) << endl;
    assert(valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ));
    assert(x0 . setCell ( CPos ( "D0" ), "10" ));
    assert(x0 . setCell ( CPos ( "D1" ), "20" ));
    assert(x0 . setCell ( CPos ( "D2" ), "30" ));
    assert(x0 . setCell ( CPos ( "D3" ), "40" ));
    assert(x0 . setCell ( CPos ( "D4" ), "50" ));
    assert(x0 . setCell ( CPos ( "E0" ), "60" ));
    assert(x0 . setCell ( CPos ( "E1" ), "70" ));
    assert(x0 . setCell ( CPos ( "E2" ), "80" ));
    assert(x0 . setCell ( CPos ( "E3" ), "90" ));
    assert(x0 . setCell ( CPos ( "E4" ), "100" ));
    assert(x0 . setCell ( CPos ( "F10" ), "=D0+5" ));
    assert(x0 . setCell ( CPos ( "F11" ), "=$D0+5" ));
    assert(x0 . setCell ( CPos ( "F12" ), "=D$0+5" ));
    assert(x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
    assert(valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ));
    res = x0.getValue(CPos("G11"));
    assert(valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ));
    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert(valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ));
    assert(x0 . setCell ( CPos ( "F0" ), "-27" ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert(valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ));
    assert(valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ));
    oss.clear();
    oss.str("");
    assert(x0 . save ( oss ));
    cout << oss.str() << endl;
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1 . load ( iss ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ));
    assert(x0 . setCell ( CPos ( "A3" ), "4e1" ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ));
    assert(valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ));
    oss.clear();
    oss.str("");
    assert(x0 . save ( oss ));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert(! x1 . load ( iss ));
    cout << "Basic prog tests passed" << endl;
}
#endif
