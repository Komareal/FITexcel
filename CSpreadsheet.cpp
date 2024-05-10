#include "CSpreadsheet.h"

#include "Cell/CPosRangeIterator.h"


CSpreadsheet::CSpreadsheet(): m_setRun(0), m_eraseRun(0) {
}

CSpreadsheet::CSpreadsheet(const CSpreadsheet &other)
    : m_sheet(other.m_sheet),
      m_setRun(other.m_setRun),
      m_eraseRun(other.m_eraseRun) {
}

CSpreadsheet::CSpreadsheet(CSpreadsheet &&other) noexcept
    : m_sheet(std::move(other.m_sheet)),
      m_setRun(other.m_setRun),
      m_eraseRun(other.m_eraseRun) {
}

CSpreadsheet &CSpreadsheet::operator=(CSpreadsheet other) {
    using namespace std;
    swap(other.m_sheet, m_sheet);
    swap(other.m_setRun, m_setRun);
    swap(other.m_eraseRun, m_eraseRun);
    return *this;
}


bool CSpreadsheet::load(std::istream &is) {
    using namespace std;
    string cellstr, whole;
    size_t controlHash;
    CSpreadsheet nexSheet;

    nexSheet.m_setRun = 1;

    whole.resize(17);
    is.read(&whole[0], 16);

    stringstream tmpis(whole);
    if (!(tmpis >> hex >> controlHash))
        return false;

    constexpr std::hash<std::string> hasher;
    tmpis = stringstream();
    tmpis << is.rdbuf();
    whole = tmpis.str();

    if (hasher(whole) != controlHash)
        return false;

    while (tmpis.good()) {
        size_t x, y, strlen;

        if (!(tmpis >> hex >> x >> y >> strlen))
            break;

        cellstr.resize(strlen);
        tmpis.ignore(1);

        tmpis.read(&cellstr[0], strlen);

        nexSheet.m_sheet.emplace(CPos(x, y), cellstr);

        tmpis.ignore(1);
    }

    if (!tmpis.eof())
        return false;

    *this = nexSheet;
    return true;
}

bool CSpreadsheet::save(std::ostream &os) const {
    using namespace std;
    constexpr char sep = ' ';
    stringstream ss;
    ss << endl;
    for (const auto &[pos, cell]: m_sheet) {
        stringstream tmp;
        cell.m_root.m_ptr->print(tmp, cell.m_refManager);

        size_t len = tmp.str().length();
        if (!cell.m_root.m_ptr->isVal())
            len += 2;
        ss << hex << pos.m_x << sep << pos.m_y << sep << len << sep;
        if (!cell.m_root.m_ptr->isVal())
            ss << "= ";
        ss << tmp.str() << endl;
    }

    constexpr hash<std::string> hasher;
    stringstream hashSs;
    hashSs << setfill('0') << setw(16) << hex << hasher(ss.str());

    os << hashSs.str() << ss.str();

    return true;
}

bool CSpreadsheet::setCell(const CPos &pos, const std::string &contents) {
    try {
        return setCell(pos, CCell(contents));
    } catch (std::invalid_argument &e) {
        // print error
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool CSpreadsheet::setCell(const CPos &pos, const CCell &cell) {
    const auto it = m_sheet.find(pos);
    try {
        if (it != m_sheet.end() && it->first == pos) {
            it->second = cell;
        } else {
            m_sheet.emplace(pos, cell);
        }
    } catch (std::invalid_argument &e) {
        // print error
        std::cout << e.what() << std::endl;
        return false;
    }
    m_setRun++;
    return true;
}

bool CSpreadsheet::eraseCell(const CPos &pos) {
    m_setRun++;
    m_eraseRun++;
    const auto it = m_sheet.find(pos);
    if (it != m_sheet.end() && it->first == pos) {
        m_sheet.erase(it);
        return true;
    }
    return false;
}

CValue CSpreadsheet::getValue(const CPos &pos) {
    const auto it = m_sheet.find(pos);
    if (it == m_sheet.end() || it->first != pos)
        return {};
    const CSharedVal res = getValue(&it->second);
    if (res == nullptr)
        return {};
    return *res;
}

CSharedVal CSpreadsheet::getValue(CCell *cell) {
    if (cell == nullptr)
        return nullptr;
    cell->m_refManager.m_base = this;
    const CSharedVal res = cell->getValue(m_setRun);
    if (res == nullptr)
        return nullptr;
    return res;
}

void CSpreadsheet::copyRect(const CPos &dst, const CPos &src, const int w, const int h) {
    std::unordered_map<CPos, CCell, CPosHash> tmp;
    const size_t xOffset = dst.m_x - src.m_x;
    const size_t yOffset = dst.m_y - src.m_y;
    auto rangeIt = CPosRangeIterator(src, CPos(src.m_x + w - 1, src.m_y + h - 1));
    for (; !rangeIt.isEnd(); ++rangeIt) {
        const auto it = m_sheet.find(rangeIt.get());
        if (it == m_sheet.end() || it->first != rangeIt.get())
            continue;
        tmp.emplace(*it);
    }
    rangeIt.reset();
    for (; !rangeIt.isEnd(); ++rangeIt) {
        CPos ofsDst = rangeIt.get().relativeMove(xOffset, yOffset);
        const auto tmpIt = tmp.find(rangeIt.get());
        if (tmpIt == tmp.end()) {
            eraseCell(ofsDst);
        } else {
            CCell &cell = tmpIt->second;
            cell.m_refManager.moveReferences(xOffset, yOffset);
            setCell(ofsDst, cell);
        }
    }
}
