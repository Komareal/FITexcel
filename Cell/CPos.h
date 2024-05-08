#ifndef CPOS_H
#define CPOS_H
#include "../header.h"


/**
 * Represents cell address like "A7" as X and Y size_t coordinates
 * X is converted from base 2 (A = 0) to base 2 (size_t)
 * Y is converted from base 10 to base 2 (size_t)
 *  Can have relative or absolute addressing, relevant only when moving;
 */
class CPos {
public:
    friend class CSpreadsheet;
    friend class CPosHash;

    // ------------ Constructors
    CPos(std::string_view str, bool ignoreFix = true);

    CPos(std::string_view str, bool ignoreColon, size_t &index);

    CPos();

    CPos(size_t x, size_t y);

    // ------------ Utils

    /**
     * Moves stored addresses according to offsets if they are relative (aren't fixed)
     * @param offsetX
     * @param offsetY
     */
    CPos relativeMove(size_t offsetX, size_t offsetY) const;

    void print(std::ostream &os) const;

    // ------------ Copy and move
    CPos(const CPos &other);

    CPos(CPos &&other) noexcept;

    CPos &operator=(CPos other);

    bool equalsWithFix(const CPos &o);

    // ------------ Equality ops
    friend bool operator==(const CPos &lhs, const CPos &rhs);

    friend bool operator!=(const CPos &lhs, const CPos &rhs);

    // ------------ Relational ops
    friend bool operator<(const CPos &lhs, const CPos &rhs);

    friend bool operator<=(const CPos &lhs, const CPos &rhs);

    friend bool operator>(const CPos &lhs, const CPos &rhs);

    friend bool operator>=(const CPos &lhs, const CPos &rhs);

private:
    /**
     * X coordinate
     */
    size_t m_x;

    /**
     * Y coordinate
     */
    size_t m_y;

    /**
     * X is relative or absolute (fixed) - setted with dollar sign in address like $A0
     */
    bool m_fixX;

    /**
     * Y is relative or absolute (fixed) - setted with dollar sign in address like A$0
     */
    bool m_fixY;

    /**
     * Parses string to size_t until it hits end or char out of base range (for this application assumes case insensitive)
     * @param src tring containing the address
     * @param base to convert to
     * @param firstChar which char to take as 0
     * @param firstNum decides what first char means. Just because of that "AA" could be 27 ('A' == 1)
     * @param index - from where to start and where it ends
     * @return
     */
    static size_t parseBase(const std::string_view &src, char base, char firstChar, int firstNum, size_t &index);


    /**
     * wrapper for parsing first (Letter) part of address with post checks, also checks for '$' before address
     * @param str string containing the address
     * @param index from where to start and where it ends
     * @param ignoreFix
     */
    void parseX(const std::string_view &str, size_t &index, bool ignoreFix);

    /**
     * wrapper for parsing second (base 10) part of address with post checks, also checks for '$' before address
     * @param str string containing the address
     * @param index from where to start and where it ends
     * @param ignore_col
     * @param ignoreFix
     */
    void parseY(const std::string_view &str, size_t &index, bool ignore_col, bool ignoreFix);
};

struct CPosHash {
    size_t operator()(const CPos &pos) const;
};
#endif //CPOS_H
