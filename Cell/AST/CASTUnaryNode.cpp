#include "CASTUnaryNode.h"

CASTUnaryNode::CASTUnaryNode(CASTNodePtr &&child, CSharedVal (*op_fnc)(const CSharedVal &), const EUnaryType type)
    : m_child(std::move(child)),
      m_type(type),
      m_opFnc(op_fnc) {
}

CASTUnaryNode::CASTUnaryNode(const CASTUnaryNode &other)
    : m_child(other.m_child),
      m_type(other.m_type),
      m_opFnc(other.m_opFnc) {
}

CASTUnaryNode::CASTUnaryNode(CASTUnaryNode &&other) noexcept
    : m_child(std::move(other.m_child)),
      m_type(std::move(other.m_type)),
      m_opFnc(std::move(other.m_opFnc)) {
}

CASTUnaryNode &CASTUnaryNode::operator=(CASTUnaryNode other) {
    using namespace std;
    swap(other.m_child, m_child);
    swap(other.m_type, m_type);
    swap(other.m_opFnc, m_opFnc);
    return *this;
}

bool CASTUnaryNode::hasValChild() const {
    return m_child.m_ptr->isVal();
}

void CASTUnaryNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    ss << " ( ";
    switch (m_type) {
        case EUnaryType::NEG:
            ss << "-";
            break;
    }
    m_child.m_ptr->print(ss, refManager);
    ss << " ) ";
}

CSharedVal CASTUnaryNode::computeVal(CRefManager &refManager) const {
    const CSharedVal res = m_child.m_ptr->computeVal(refManager);
    if (res == nullptr)
        return nullptr;
    return m_opFnc(res);
}
