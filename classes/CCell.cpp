#include "CCell.h"

#include "CBuilder.h"
// ------------ Constructors
CCell::CCell(std::string_view str): m_computedAt(0) {
    CBuilder builder(str, m_references);
    parseExpression(std::string(str), builder);
    m_root = builder.getRoot();
}


// ------------ Move and copy


CCell::CCell(const CCell &other)
    : m_computedValue(other.m_computedValue),
      m_computedAt(other.m_computedAt),
      m_root(std::make_unique<CASTNode>(*other.m_root)),
      m_references(other.m_references) {
}

CCell::CCell(CCell &&other) noexcept : m_computedAt(0) {
    std::swap(other.m_computedValue, m_computedValue);
    std::swap(other.m_computedAt, m_computedAt);
    std::swap(other.m_root, m_root);
    std::swap(other.m_references, m_references);
}

CCell &CCell::operator=(CCell other) {
    std::swap(other.m_computedValue, m_computedValue);
    std::swap(other.m_computedAt, m_computedAt);
    std::swap(other.m_root, m_root);
    std::swap(other.m_references, m_references);
    return *this;
}