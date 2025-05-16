#include "CASTRefNode.h"
#include "../CRefManager.h"
CASTRefNode::CASTRefNode(const size_t ref): m_ref(ref) {
}

CASTRefNode::CASTRefNode(const CASTRefNode &other): m_ref(other.m_ref) {
}

CASTRefNode::CASTRefNode(CASTRefNode &&other) noexcept: m_ref(std::move(other.m_ref)) {
}

CASTRefNode &CASTRefNode::operator=(CASTRefNode other) {
    std::swap(other.m_ref, m_ref);
    return *this;
}

void CASTRefNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    refManager.print(ss, m_ref);
}

CSharedVal CASTRefNode::computeVal(CRefManager &refManager) const {
    return refManager.getValOfReference(m_ref);
}
