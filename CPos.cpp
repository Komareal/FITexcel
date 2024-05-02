#include "CPos.h"
// ------------ Constructors
CPos::CPos(const std::string_view str): m_x(0), m_y(0) {
    size_t index = 0;
    m_x = parseBase(str, 26, 'A', index);
    if (index == 0 || index == str.length())
        throw std::invalid_argument("CPos: Invalid cell address");

    m_y = parseBase(str, 10, '0', index);
    if (index != str.length())
        throw std::invalid_argument("CPos: Invalid cell address");
}

size_t CPos::parseBase(const std::string_view &src, const char base, const char zeroChar, size_t &index) {
    // Argument checks
    if (base < 2)
        throw std::invalid_argument("ParseBase: Base has to be more than 2");

    if (base + zeroChar > CHAR_MAX)
        throw std::invalid_argument("ParseBase: Base/zero character combination, overflows char");

    if (index >= src.length())
        throw std::out_of_range("ParseBase: Index out of range");

    // Unless we hit out of range number, iterate through the string
    // In each iteration multiply res by base and add current char converted to base 2
    size_t res = 0;

    while (index < src.length()) {
        if (src[index] < zeroChar)
            return res;

        int n = src[index] - zeroChar;

        // convert lowercase to uppercase
        if (n >= 'a' && n <= 'z')
            n -= 'A' - 'a';

        if (n > zeroChar + base)
            return res;

        // check res overflow
        if (res > SIZE_MAX / base)
            throw std::overflow_error("ParseBase: Value in string to convert is bigger than current result size");
        res *= base;

        // check res overflow
        if (res > SIZE_MAX - n)
            throw std::overflow_error("ParseBase: Value in string to convert is bigger than current result size");
        res += n;
        index++;
    }
    return res;
}

// ------------ Copy and move
CPos::CPos(const CPos &other) = default;

CPos::CPos(CPos &&other) noexcept: m_x(other.m_x), m_y(other.m_y) {
}

CPos &CPos::operator=(CPos other) {
    std::swap(other.m_x, m_x);
    std::swap(other.m_y, m_y);
    return *this;
}


// ------------ Equality ops
bool operator==(const CPos &lhs, const CPos &rhs) {
    return std::tie(lhs.m_x, lhs.m_y) == std::tie(rhs.m_x, rhs.m_y);
}

bool operator!=(const CPos &lhs, const CPos &rhs) {
    return !(lhs == rhs);
}

// ------------ Relational ops
bool operator<(const CPos &lhs, const CPos &rhs) {
    return std::tie(lhs.m_x, lhs.m_y) < std::tie(rhs.m_x, rhs.m_y);
}

bool operator>(const CPos &lhs, const CPos &rhs) {
    return rhs < lhs;
}

bool operator>=(const CPos &lhs, const CPos &rhs) {
    return !(lhs < rhs);
}

bool operator<=(const CPos &lhs, const CPos &rhs) {
    return rhs >= lhs;
}
