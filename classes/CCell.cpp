#include "CCell.h"

#include "CASTProccesser.h"
#include "CBuilder.h"

// ------------ Constructors
CCell::CCell(std::string_view str)
    : m_valueValidAt(0),
      m_stateGivenAt(0),
      m_state(ECellState::FRESH) {
    CBuilder builder(str, m_references);
    parseExpression(std::string(str), builder);
    m_root = builder.getRoot();
}


// ------------ Move and copy


CCell::CCell(const CCell &other)
    : m_computedValue(other.m_computedValue),
      m_valueValidAt(other.m_valueValidAt),
      m_stateGivenAt(other.m_stateGivenAt),
      m_state(other.m_state),
      m_root(std::make_unique<CASTNode>(*other.m_root)),
      m_references(other.m_references) {
}

CCell::CCell(CCell &&other) noexcept : m_valueValidAt(0), m_stateGivenAt(0), m_state(ECellState::FRESH) {
    using std::swap;
    swap(other.m_computedValue, m_computedValue);
    swap(other.m_valueValidAt, m_valueValidAt);
    swap(other.m_root, m_root);
    swap(other.m_references, m_references);
    swap(other.m_state, m_state);
    swap(other.m_stateGivenAt, m_stateGivenAt);
}

CCell &CCell::operator=(CCell other) {
    using std::swap;
    swap(other.m_computedValue, m_computedValue);
    swap(other.m_valueValidAt, m_valueValidAt);
    swap(other.m_root, m_root);
    swap(other.m_references, m_references);
    swap(other.m_state, m_state);
    swap(other.m_stateGivenAt, m_stateGivenAt);
    return *this;
}

CSharedVal CCell::getValue(size_t setRun, size_t getRun, std::map<CPos, CCell> &map) {
    if (m_valueValidAt == setRun)
        return m_computedValue;
    CSharedVal res;
    if (getRun == m_stateGivenAt && m_state == CCell::ECellState::OPEN) {
        saveVal(res, setRun);
        return res;
    }

    m_state = CCell::ECellState::OPEN;
    m_stateGivenAt = getRun;
    CASTProccesser proccesser(setRun, getRun, map, m_references);
    res = proccesser.getValue(*m_root);
    saveVal(res, setRun);
    return res;
}


void CCell::saveVal(const CSharedVal &val, const size_t setRun) {
    m_computedValue = val;
    m_valueValidAt = setRun;
    m_state = ECellState::CLOSED;
}
