#include "CSpreadsheet.h"

CSpreadsheet::CSpreadsheet(): m_setRun(0), m_getRun(0) {
}

//
// bool CSpreadsheet::load(std::istream &is) {
// }
//
// bool CSpreadsheet::save(std::ostream &os) const {
// }

bool CSpreadsheet::setCell(CPos pos, std::string contents) {
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
