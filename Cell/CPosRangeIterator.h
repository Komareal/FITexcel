#ifndef CPOSRANGE_H
#define CPOSRANGE_H
#include "CPos.h"


class CPosRangeIterator {
public:
    CPosRangeIterator(const CPos &start, const CPos &end);

    CPosRangeIterator(const CPosRangeIterator &other);

    CPosRangeIterator(CPosRangeIterator &&other) noexcept;

    CPosRangeIterator &operator=(CPosRangeIterator other);

    bool isEnd() const;

    void reset();

    CPosRangeIterator &operator++();

    CPos get() const;

private:
    size_t m_activeXOffset;
    size_t m_activeYOffset;
    CPos m_leftUpper;
    CPos m_rightLower;
};
#endif //CPOSRANGE_H
