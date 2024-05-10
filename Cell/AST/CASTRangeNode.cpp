#include "CASTRangeNode.h"

#include "../CRefManager.h"

CASTRangeNode::CASTRangeNode(const size_t left_reference, const size_t right_reference)
    : m_leftReference(left_reference),
      m_rightReference(right_reference) {
}

CASTRangeNode::CASTRangeNode(const CASTRangeNode &other)
    : m_leftReference(other.m_leftReference),
      m_rightReference(other.m_rightReference) {
}

CASTRangeNode::CASTRangeNode(CASTRangeNode &&other) noexcept
    : m_leftReference(std::move(other.m_leftReference)),
      m_rightReference(std::move(other.m_rightReference)) {
}

CASTRangeNode &CASTRangeNode::operator=(CASTRangeNode other) {
    std::swap(other.m_leftReference, m_leftReference);
    std::swap(other.m_rightReference, m_leftReference);
    return *this;
}

void CASTRangeNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    refManager.print(ss, m_leftReference);
    ss << ':';
    refManager.print(ss, m_rightReference);
}

CSharedVal CASTRangeNode::computeVal(CRefManager &refManager) const {
    return nullptr;
}

std::optional<CPosRangeIterator> CASTRangeNode::getRangeIterator(CRefManager &refManager) const {
    return CPosRangeIterator(refManager.getPosOfReference(m_leftReference), refManager.getPosOfReference(m_rightReference));
}
