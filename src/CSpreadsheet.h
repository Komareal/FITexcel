#ifndef CSPREADSHEET_H
#define CSPREADSHEET_H
#include "header.h"

#include "Cell/CCell.h"

#ifndef __PROGTEST__
constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif

class CSpreadsheet {
public:
    friend class CRefManager;

    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_SPEED | SPREADSHEET_FILE_IO | SPREADSHEET_FUNCTIONS;
    }

    CSpreadsheet();

    CSpreadsheet(const CSpreadsheet &other);

    CSpreadsheet(CSpreadsheet &&other) noexcept;

    CSpreadsheet &operator=(CSpreadsheet other);

    /**
     * Rebuilds the spreadsheet from the input stream
     * needs same format as save produces
     *
     * @param is
     * @return
     */
    bool load(std::istream &is);

    /**
     * Saves the spreadsheet to the output stream
     * as a text, where each cell is printed like
     * X Y LEN CONTENT
     * adds checksum at the beginning
     * @param os
     * @return
     */
    bool save(std::ostream &os) const;

    /**
     *  Makes new cell on the position (replaces the old one)
     * @param pos  position of the cell
     * @param contents  contents of the cell
     * @return  true if the cell was made (contents were valid and the cell was not cyclic)
     */
    bool setCell(const CPos &pos, const std::string &contents);

    /**
     *  saves the cell to the spreadsheet (replaces the old one)
     * @param pos  position of the cell
     * @param cell  cell to save
     * @return true if the cell was saved
     */
    bool setCell(const CPos &pos, const CCell &cell);

    /**
     *  Erases the cell on the position
     * @param pos  position of the cell
     * @return  true if the cell was erased
     */
    bool eraseCell(const CPos &pos);

    /**
     *  Returns the value of the cell on the position
     * @param pos  position of the cell
     * @return  value of the cell
     */
    CValue getValue(const CPos &pos);

    /**
     *  Returns the value of the cell
     * @param cell  cell to get the value from
     * @return shared_ptr  value of the cell
     */
    CSharedVal getValue(CCell *cell);

    /**
     *  Copies the rectangle from the source to the destination
     * @param dst
     * @param src
     * @param w Width
     * @param h  Height
     */
    void copyRect(const CPos &dst,
                  const CPos &src,
                  int w = 1,
                  int h = 1);

private:
    /**
     *  Spreadsheet data
     */
    std::unordered_map<CPos, CCell, CPosHash> m_sheet;
    /**
     *  Run counter for the set operations
     *  Needed for cell value caching and cycle detection
     */
    size_t m_setRun;
    /**
     *  Run counter for the erase operations
     *  Needed for cell reference caching
     */
    size_t m_eraseRun;
};


#endif //CSPREADSHEET_H
