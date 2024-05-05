#include "CSpreadsheet.h"

CSpreadsheet::CSpreadsheet(): m_setRun(0), m_getRun(0) {
}

CSpreadsheet::CSpreadsheet(const CSpreadsheet &other)
    : m_sheet(other.m_sheet),
      m_setRun(other.m_setRun),
      m_getRun(other.m_getRun) {
}

CSpreadsheet::CSpreadsheet(CSpreadsheet &&other) noexcept
    : m_sheet(std::move(other.m_sheet)),
      m_setRun(other.m_setRun),
      m_getRun(other.m_getRun) {
}

CSpreadsheet &CSpreadsheet::operator=(CSpreadsheet other) {
    using namespace std;
    swap(other.m_sheet, m_sheet);
    swap(other.m_setRun, m_setRun);
    swap(other.m_getRun, m_getRun);
    return *this;
}

//
// bool CSpreadsheet::load(std::istream &is) {
// }
//
// bool CSpreadsheet::save(std::ostream &os) const {
// }

bool CSpreadsheet::setCell(const CPos& pos, const std::string& contents) {
    m_setRun++;
    const auto it = m_sheet.lower_bound(pos);
    if (it != m_sheet.end() && it->first == pos) {
        it->second = CCell(contents);
    } else
        m_sheet.emplace_hint(it, pos, contents);
    return true;
}

CValue CSpreadsheet::getValue(const CPos &pos) {
    m_getRun++;
    const auto it = m_sheet.lower_bound(pos);
    if (it == m_sheet.end() || it->first != pos)
        return {};
    const CSharedVal res = it->second.getValue(m_setRun, m_getRun, m_sheet);
    if (res == nullptr)
        return {};
    return *res;
}

//
// void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
// }
