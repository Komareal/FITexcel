#include "CASTNode.h"

CASTNode::CASTNode(CASTNodeVal val)
    : m_val(std::move(val)),
      m_childern({}),
      m_childernSize(0) {
}

CASTNode::CASTNode(const CASTNode &other)
    : m_val(other.m_val),
      m_childern({}),
      m_childernSize(other.m_childernSize) {
    for (size_t i = 0; i < 3; i++) {
        if (i < m_childernSize)
            m_childern[i] = std::make_unique<CASTNode>(*other.m_childern[i]);
    }
}

CASTNode::CASTNode(CASTNode &&other) noexcept : m_val(std::move(other.m_val)), m_childern({}), m_childernSize(other.m_childernSize) {
    std::swap(m_childern, other.m_childern);
}

CASTNode &CASTNode::operator=(CASTNode other) {
    using std::swap;
    swap(m_childern, other.m_childern);
    swap(m_childernSize, other.m_childernSize);
    swap(m_val, other.m_val);
    return *this;
}
