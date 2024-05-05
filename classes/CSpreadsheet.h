#ifndef CSPREADSHEET_H
#define CSPREADSHEET_H
#include "../header.h"

#include "CCell.h"
constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

class CSpreadsheet {
public:
    static unsigned capabilities() {
        return 0;
    }

    CSpreadsheet();

    CSpreadsheet(const CSpreadsheet &other);

    CSpreadsheet(CSpreadsheet &&other) noexcept;

    CSpreadsheet & operator=(CSpreadsheet other);

    bool load(std::istream &is);

    bool save(std::ostream &os) const;

    bool setCell(const CPos& pos,
                 const std::string& contents);

    CValue getValue(const CPos &pos);

    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

private:
    std::map<CPos, CCell> m_sheet;
    size_t m_setRun;
    size_t m_getRun;
};


#endif //CSPREADSHEET_H
