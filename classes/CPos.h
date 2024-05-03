#ifndef CPOS_H
#define CPOS_H


/**
 * Represents cell address like "A7" as X and Y size_t coordinates
 * X is converted from base 2 (A = 0) to base 2 (size_t)
 * Y is converted from base 10 to base 2 (size_t)
 */
class CPos {
public:
    // ------------ Constructors
    CPos(std::string_view str);

    // ------------ Copy and move
    CPos(const CPos &other);

    CPos(CPos &&other) noexcept;

    CPos &operator=(CPos other);

    // ------------ Equality ops
    friend bool operator==(const CPos &lhs, const CPos &rhs);

    friend bool operator!=(const CPos &lhs, const CPos &rhs);

    // ------------ Relational ops
    friend bool operator<(const CPos &lhs, const CPos &rhs);

    friend bool operator<=(const CPos &lhs, const CPos &rhs);

    friend bool operator>(const CPos &lhs, const CPos &rhs);

    friend bool operator>=(const CPos &lhs, const CPos &rhs);

private:
    size_t m_x;
    size_t m_y;


    /**
     * Parses string to size_t until it hits end or char out of base range (for this application assumes case insensitive)
     * @param src string
     * @param base to convert to
     * @param zeroChar which char to take as 0
     * @param index - from where to start and where it ends
     * @return
     */
    static size_t parseBase(const std::string_view &src, char base, char zeroChar, size_t &index);
};

#endif //CPOS_H
