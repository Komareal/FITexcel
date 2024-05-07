#include "CASTBinaryNode.h"

CASTBinaryNode::CASTBinaryNode(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), const EBinaryType type)
    : m_childern{},
      m_chSize(0),
      m_type(type),
      m_opFnc(op_fnc) {
}

CASTBinaryNode::CASTBinaryNode(const CASTBinaryNode &other)
    : m_childern{},
      m_chSize(other.m_chSize),
      m_type(other.m_type),
      m_opFnc(other.m_opFnc) {
    for (size_t i = 0; i < m_chSize; i++)
        m_childern[i] = other.m_childern[i];
}

CASTBinaryNode::CASTBinaryNode(CASTBinaryNode &&other) noexcept
    : m_childern{},
      m_chSize(std::move(other.m_chSize)),
      m_type(std::move(other.m_type)),
      m_opFnc(std::move(other.m_opFnc)) {
    for (size_t i = 0; i < m_chSize; i++)
        m_childern[i] = std::move(other.m_childern[i]);
}

CASTBinaryNode &CASTBinaryNode::operator=(CASTBinaryNode other) {
    using namespace std;
    swap(m_childern, other.m_childern);
    swap(m_chSize, other.m_chSize);
    swap(m_type, other.m_type);
    swap(m_opFnc, other.m_opFnc);
    return *this;
}

void CASTBinaryNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    if (m_chSize != 2)
        throw std::out_of_range("Binary op does not have enough childern for printing");

    ss << " ( ";
    m_childern[1].m_ptr->print(ss, refManager);
    ss << " ";
    switch (m_type) {
        case EBinaryType::ADD:
            ss << "+";
            break;
        case EBinaryType::SUB:
            ss << "-";
            break;
        case EBinaryType::MUL:
            ss << "*";
            break;
        case EBinaryType::DIV:
            ss << "/";
            break;
        case EBinaryType::POW:
            ss << "^";
            break;
        case EBinaryType::EQ:
            ss << "=";
            break;
        case EBinaryType::NE:
            ss << "!=";
            break;
        case EBinaryType::LT:
            ss << "<";
            break;
        case EBinaryType::LE:
            ss << "<=";
            break;
        case EBinaryType::GT:
            ss << ">";
            break;
        case EBinaryType::GE:
            ss << ">=";
            break;
    }
    ss << " ";
    m_childern[0].m_ptr->print(ss, refManager);
    ss << " ) ";
}

CSharedVal CASTBinaryNode::computeVal(CRefManager &refManager) const {
    if (m_chSize != 2)
        throw std::out_of_range("Binary op does not have enough childern for printing");

    const CSharedVal first = m_childern[1].m_ptr->computeVal(refManager),
            second = m_childern[0].m_ptr->computeVal(refManager);

    if (first == nullptr || second == nullptr)
        return nullptr;

    return m_opFnc(first, second);
}

void CASTBinaryNode::addChild(const CASTNodePtr &child) {
    if (m_chSize == 2)
        return;
    m_childern[m_chSize++] = child;
}
