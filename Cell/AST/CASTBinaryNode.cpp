#include "CASTBinaryNode.h"

CASTBinaryNode::CASTBinaryNode(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), const EBinaryType type)
    : m_childern{},
      m_childernSize(0),
      m_type(type),
      m_opFnc(op_fnc) {
}

CASTBinaryNode::CASTBinaryNode(const CASTBinaryNode &other)
    : m_childern{},
      m_childernSize(other.m_childernSize),
      m_type(other.m_type),
      m_opFnc(other.m_opFnc) {
    for (size_t i = 0; i < m_childernSize; i++)
        m_childern[i] = other.m_childern[i];
}

CASTBinaryNode::CASTBinaryNode(CASTBinaryNode &&other) noexcept
    : m_childern{},
      m_childernSize(std::move(other.m_childernSize)),
      m_type(std::move(other.m_type)),
      m_opFnc(std::move(other.m_opFnc)) {
    for (size_t i = 0; i < m_childernSize; i++)
        m_childern[i] = std::move(other.m_childern[i]);
}

CASTBinaryNode &CASTBinaryNode::operator=(CASTBinaryNode other) {
    using namespace std;
    swap(m_childern, other.m_childern);
    swap(m_childernSize, other.m_childernSize);
    swap(m_type, other.m_type);
    swap(m_opFnc, other.m_opFnc);
    return *this;
}

void CASTBinaryNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    if (m_childernSize != 2)
        throw std::out_of_range("Binary op does not have enough childern for printing");

    ss << "( ";
    m_childern[1].m_ptr->print(ss, refManager);
    ss << " ";
    ss << m_stringMap.find(m_type)->second;
    ss << " ";
    m_childern[0].m_ptr->print(ss, refManager);
    ss << " )";
}

CSharedVal CASTBinaryNode::computeVal(CRefManager &refManager) const {
    if (m_childernSize != 2)
        throw std::out_of_range("Binary op does not have enough childern for printing");

    const CSharedVal first = m_childern[1].m_ptr->computeVal(refManager),
            second = m_childern[0].m_ptr->computeVal(refManager);

    if (first == nullptr || second == nullptr)
        return nullptr;

    return m_opFnc(first, second);
}

void CASTBinaryNode::addChild(const CASTNodePtr &child) {
    if (m_childernSize == 2)
        return;
    m_childern[m_childernSize++] = child;
}

bool CASTBinaryNode::hasValChildern() const {
    if (m_childernSize != 2)
        throw std::out_of_range("Binary op does not have enough childern for checking them");
    return m_childern[0].m_ptr->isVal() && m_childern[1].m_ptr->isVal();
}
