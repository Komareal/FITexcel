#include "CCell.h"

#include "CASTProccesser.h"
#include "CBuilder.h"

// ------------ Constructors
CCell::CCell(std::string_view str)
    : m_valueValidAt(0),
      m_ptrCacheValidAt(0),
      m_state(ECellState::FRESH) {
    CBuilder builder(str, m_references);
    parseExpression(std::string(str), builder);
    m_root = builder.getRoot();
}


// ------------ Move and copy


CCell::CCell(const CCell &other)
    : m_computedValue(other.m_computedValue),
      m_valueValidAt(other.m_valueValidAt),
      m_ptrCacheValidAt(other.m_ptrCacheValidAt),
      m_state(other.m_state),
      m_root(std::make_unique<CASTNode>(*other.m_root)) {
    for (const auto &[oPos, oRef]: other.m_references) {
        m_references.emplace_back(oPos, nullptr);
    }
}

CCell::CCell(CCell &&other) noexcept : m_valueValidAt(0), m_ptrCacheValidAt(0), m_state(ECellState::FRESH) {
    using std::swap;
    swap(other.m_computedValue, m_computedValue);
    swap(other.m_valueValidAt, m_valueValidAt);
    swap(other.m_root, m_root);
    swap(other.m_references, m_references);
    swap(other.m_state, m_state);
    swap(other.m_ptrCacheValidAt, m_ptrCacheValidAt);
}

CCell &CCell::operator=(CCell other) {
    using std::swap;
    swap(other.m_computedValue, m_computedValue);
    swap(other.m_valueValidAt, m_valueValidAt);
    swap(other.m_root, m_root);
    swap(other.m_references, m_references);
    swap(other.m_state, m_state);
    swap(other.m_ptrCacheValidAt, m_ptrCacheValidAt);
    return *this;
}


CSharedVal CCell::getValue(const size_t setRun, const size_t eraseRun, std::map<CPos, CCell> &map) {
    CSharedVal res;
    if (m_valueValidAt == setRun) {
        if (m_state == CCell::ECellState::OPEN) {
            saveVal(res);
            return res;
        }
        return m_computedValue;
    }

    m_valueValidAt = setRun;
    m_state = CCell::ECellState::OPEN;
    CASTProccesser proccesser(setRun, eraseRun, m_ptrCacheValidAt, map, m_references);
    res = proccesser.getValue(*m_root);
    saveVal(res);
    return res;
}

void CCell::moveReferences(const size_t x, const size_t y) {
    for (auto it = m_references.begin(); it != m_references.end(); ++it) {
        *it = {it->first.relativeMove(x,y),nullptr};
    }
}


void CCell::saveVal(const CSharedVal &val) {
    m_computedValue = val;
    m_state = ECellState::CLOSED;
}
