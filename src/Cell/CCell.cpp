#include "CCell.h"

#include "CBuilder.h"

// ------------ Constructors
CCell::CCell(const std::string_view str)
    : m_valueValidAt(0),
      m_state(ECellState::FRESH) {
    CBuilder builder(m_refManager);
    parseExpression(std::string(str), builder);
    m_root = builder.getRoot();
}


// ------------ Move and copy


CCell::CCell(const CCell &other)
    : m_computedValue(other.m_computedValue),
      m_valueValidAt(other.m_valueValidAt),
      m_refManager(other.m_refManager),
      m_state(other.m_state),
      m_root(other.m_root) {
}

CCell::CCell(CCell &&other) noexcept
    : m_computedValue(std::move(other.m_computedValue)),
      m_valueValidAt(std::move(other.m_valueValidAt)),
      m_refManager(std::move(other.m_refManager)),
      m_state(std::move(other.m_state)),
      m_root(std::move(other.m_root)) {
}

CCell &CCell::operator=(CCell other) {
    using std::swap;
    swap(other.m_computedValue, m_computedValue);
    swap(other.m_valueValidAt, m_valueValidAt);
    swap(other.m_root, m_root);
    swap(other.m_state, m_state);
    swap(other.m_refManager, m_refManager);
    return *this;
}


CSharedVal CCell::getValue(const size_t run) {
    CSharedVal res;
    if (m_valueValidAt == run) {
        if (m_state == ECellState::OPEN) {
            saveVal(res);
            return res;
        }
        return m_computedValue;
    }

    m_valueValidAt = run;
    m_state = ECellState::OPEN;
    res = m_root->computeVal(m_refManager);
    saveVal(res);
    return res;
}

void CCell::saveVal(const CSharedVal &val) {
    m_computedValue = val;
    m_state = ECellState::CLOSED;
}
