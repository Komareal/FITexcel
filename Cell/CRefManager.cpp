#include "CRefManager.h"
#include "../CSpreadsheet.h"

CRefManager::CRefManager(): m_base(nullptr), m_validAt(0) {
}

CRefManager::CRefManager(const CRefManager &other)
    : m_base(nullptr),
      m_validAt(other.m_validAt) {
    for (const auto &[oPos, oRef]: other.m_references) {
        m_references.emplace_back(oPos, nullptr);
    }
}

CRefManager::CRefManager(CRefManager &&other) noexcept
    : m_base(nullptr),
      m_validAt(other.m_validAt),
      m_references(std::move(other.m_references)) {
}

CRefManager &CRefManager::operator=(CRefManager other) {
    std::swap(other.m_validAt, m_validAt);
    std::swap(other.m_references, m_references);
    return *this;
}

size_t CRefManager::addReference(const CPos &pos) {
    for (size_t i = 0; i < m_references.size(); i++)
        if (m_references[i].first.equalsWithFix(pos))
            return i;

    m_references.emplace_back(pos, nullptr);
    return m_references.size() - 1;
}

CCell *CRefManager::getReference(const size_t i) {
    auto [pos, ptr] = m_references[i];
    if (ptr == nullptr || m_base->m_setRun != m_validAt) {
        const auto it = m_base->m_sheet.find(pos);
        if (it != m_base->m_sheet.end()) {
            ptr = &it->second;
            m_references[i] = std::pair(pos, ptr);
        } else {
            return nullptr;
        }
    }
    return ptr;
}

CSharedVal CRefManager::getValOfReference(const size_t i) {
    CCell *ptr = getReference(i);
    if (ptr == nullptr)
        return nullptr;
    return m_base->getValue(ptr);
}

void CRefManager::print(std::ostream &os, const size_t i) const {
    m_references[i].first.print(os);
}

void CRefManager::moveReferences(const size_t x, const size_t y) {
    for (auto it = m_references.begin(); it != m_references.end(); ++it) {
        *it = {it->first.relativeMove(x, y), nullptr};
    }
}
