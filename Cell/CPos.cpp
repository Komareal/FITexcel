#include "CPos.h"
// ------------ Constructors
CPos::CPos(const std::string_view str, const bool ignoreFix): m_x(0), m_y(0), m_fixX(false), m_fixY(false) {
    size_t index = 0;
    parseX(str, index, ignoreFix);
    parseY(str, index, false, ignoreFix);
}

CPos::CPos(const std::string_view str, const bool ignoreColon, size_t &index): m_x(0), m_y(0), m_fixX(false), m_fixY(false) {
    parseX(str, index, true);
    parseY(str, index, ignoreColon, true);
}

CPos::CPos(): m_x(0), m_y(0), m_fixX(false), m_fixY(false) {
}

CPos::CPos(const size_t x, const size_t y): m_x(x), m_y(y), m_fixX(false), m_fixY(false) {
}

void CPos::parseX(const std::string_view &str, size_t &index, const bool ignoreFix) {
    if (!ignoreFix && str[index] == '$') {
        m_fixX = true;
        index++;
    }

    m_x = parseBase(str, 26, 'A', 1, index);
    if (index == 0 || index >= str.length())
        throw std::invalid_argument("CPos: Invalid cell address");
}

void CPos::parseY(const std::string_view &str, size_t &index, const bool ignore_col, const bool ignoreFix) {
    if (!ignoreFix && str[index] == '$') {
        m_fixY = true;
        index++;
    }

    m_y = parseBase(str, 10, '0', 0, index);
    if (index != str.length() && (!ignore_col || str[index] == ':'))
        throw std::invalid_argument("CPos: Invalid cell address");
}

size_t CPos::parseBase(const std::string_view &src, const char base, const char firstChar, const int firstNum, size_t &index) {
    // Argument checks
    if (base < 2)
        throw std::invalid_argument("ParseBase: Base has to be more than 2");

    if (base + firstChar > CHAR_MAX)
        throw std::invalid_argument("ParseBase: Base/zero character combination, overflows char");

    if (index >= src.length())
        throw std::invalid_argument("ParseBase: Index out of range");

    // Unless we hit out of range number, iterate through the string
    // In each iteration multiply res by base and add current char converted to base 2
    size_t res = 0;

    while (index < src.length()) {
        if (src[index] < firstChar)
            return res;

        int n = src[index];
        // convert lowercase to uppercase
        if (n >= 'a' && n <= 'z')
            n -= 'a' - 'A';
        n = n - firstChar + firstNum;


        if (n > firstChar + base)
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

// ------------ Utils
CPos CPos::relativeMove(const size_t offsetX, const size_t offsetY) const {
    CPos res = *this;

    if (!m_fixX) {
        res.m_x += offsetX;
    }

    if (!m_fixY) {
        res.m_y += offsetY;
    }
    return res;
}


void CPos::print(std::ostream &os) const {
    os << " ";
    if (m_fixX) {
        os << "$";
    }

    if (m_x == 0) {
        os << "A";
    } else {
        size_t tmpX = m_x;
        std::string tmpstr;
        while (tmpX > 0) {
            tmpstr = static_cast<char>('A' + (tmpX % 26) - 1) + tmpstr;
            tmpX /= 26;
        }
        os << tmpstr;
    }
    if (m_fixY) {
        os << "$";
    }

    os << m_y;
    os << " ";
}

// ------------ Copy and move
CPos::CPos(const CPos &other) = default;

CPos::CPos(CPos &&other) noexcept: m_x(std::move(other.m_x)), m_y(std::move(other.m_y)), m_fixX(std::move(other.m_fixX)), m_fixY(std::move(other.m_fixY)) {
}

CPos &CPos::operator=(CPos other) {
    std::swap(other.m_x, m_x);
    std::swap(other.m_y, m_y);
    std::swap(other.m_fixX, m_fixX);
    std::swap(other.m_fixY, m_fixY);
    return *this;
}


// ------------ Equality ops

bool CPos::equalsWithFix(const CPos &o) {
    return std::tie(m_x, m_y, m_fixX, m_fixY) == std::tie(o.m_x, o.m_y, o.m_fixX, o.m_fixY);
}

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
