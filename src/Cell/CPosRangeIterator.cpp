#include "CPosRangeIterator.h"

CPosRangeIterator::CPosRangeIterator(const CPos &start, const CPos &end)
    : m_activeXOffset(0),
      m_activeYOffset(0),
      m_leftUpper(CPos(std::min(start.m_x, end.m_x), std::min(start.m_y, end.m_y))),
      m_rightLower(CPos(std::max(start.m_x, end.m_x), std::max(start.m_y, end.m_y))) {
}

CPosRangeIterator::CPosRangeIterator(const CPosRangeIterator &other)
    : m_activeXOffset(other.m_activeXOffset),
      m_activeYOffset(other.m_activeYOffset),
      m_leftUpper(other.m_leftUpper),
      m_rightLower(other.m_rightLower) {
}

CPosRangeIterator::CPosRangeIterator(CPosRangeIterator &&other) noexcept
    : m_activeXOffset(std::move(other.m_activeXOffset)),
      m_activeYOffset(std::move(other.m_activeYOffset)),
      m_leftUpper(std::move(other.m_leftUpper)),
      m_rightLower(std::move(other.m_rightLower)) {
}

CPosRangeIterator &CPosRangeIterator::operator=(CPosRangeIterator other) {
    using std::swap;
    swap(m_activeXOffset, other.m_activeXOffset);
    swap(m_activeYOffset, other.m_activeYOffset);
    swap(m_leftUpper, other.m_leftUpper);
    swap(m_rightLower, other.m_rightLower);
    return *this;
}

bool CPosRangeIterator::isEnd() const {
    return m_activeYOffset > m_rightLower.m_y - m_leftUpper.m_y;
}

void CPosRangeIterator::reset() {
    m_activeXOffset = 0;
    m_activeYOffset = 0;
}

CPosRangeIterator &CPosRangeIterator::operator++() {
    if (m_activeXOffset < m_rightLower.m_x - m_leftUpper.m_x) {
        m_activeXOffset++;
    } else {
        m_activeXOffset = 0;
        m_activeYOffset++;
    }
    return *this;
}

CPos CPosRangeIterator::get() const {
    return m_leftUpper.relativeMove(m_activeXOffset, m_activeYOffset);
}
