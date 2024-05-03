#ifndef CSPREADSHEET_H
#define CSPREADSHEET_H


class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    CSpreadsheet();

    bool load(std::istream &is);

    bool save(std::ostream &os) const;

    bool setCell(CPos pos,
                 std::string contents);

    CValue getValue(CPos pos);

    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

private:
    // todo
};


#endif //CSPREADSHEET_H
