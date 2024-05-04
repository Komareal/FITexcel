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
    m_sheet.emplace(pos, contents);
    return true;
}

CValue CSpreadsheet::getValue(const CPos &pos) {
    m_getRun++;
    const CSharedVal res = m_sheet.at(pos).getValue(m_setRun, m_getRun, m_sheet);
    if (res == nullptr)
        return {};
    return *res;
}

//
// void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
// }
