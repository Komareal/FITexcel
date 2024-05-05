#include "CSpreadsheet.h"

CSpreadsheet::CSpreadsheet(): m_setRun(0), m_eraseRun(0) {
}

CSpreadsheet::CSpreadsheet(const CSpreadsheet &other)
    : m_sheet(other.m_sheet),
      m_setRun(other.m_setRun),
      m_eraseRun(other.m_eraseRun) {
}

CSpreadsheet::CSpreadsheet(CSpreadsheet &&other) noexcept
    : m_sheet(std::move(other.m_sheet)),
      m_setRun(other.m_setRun),
      m_eraseRun(other.m_eraseRun) {
}

CSpreadsheet &CSpreadsheet::operator=(CSpreadsheet other) {
    using namespace std;
    swap(other.m_sheet, m_sheet);
    swap(other.m_setRun, m_setRun);
    swap(other.m_eraseRun, m_eraseRun);
    return *this;
}

//
// bool CSpreadsheet::load(std::istream &is) {
// }
//
// bool CSpreadsheet::save(std::ostream &os) const {
// }

bool CSpreadsheet::setCell(const CPos &pos, const std::string &contents) {
    m_setRun++;
    const auto it = m_sheet.lower_bound(pos);
    if (it != m_sheet.end() && it->first == pos) {
        it->second = CCell(contents);
    } else {
        m_sheet.emplace_hint(it, pos, contents);
    }
    return true;
}

bool CSpreadsheet::setCell(const CPos &pos, const CCell &cell) {
    m_setRun++;
    const auto it = m_sheet.lower_bound(pos);
    if (it != m_sheet.end() && it->first == pos) {
        it->second = cell;
    } else {
        m_sheet.emplace_hint(it, pos, cell);
    }
    return true;
}

bool CSpreadsheet::eraseCell(const CPos &pos) {
    m_setRun++;
    const auto it = m_sheet.lower_bound(pos);
    if (it != m_sheet.end() && it->first == pos) {
        m_sheet.erase(it);
        return true;
    }
    return false;
}

CValue CSpreadsheet::getValue(const CPos &pos) {
    const auto it = m_sheet.lower_bound(pos);
    if (it == m_sheet.end() || it->first != pos)
        return {};
    const CSharedVal res = it->second.getValue(m_setRun, m_eraseRun, m_sheet);
    if (res == nullptr)
        return {};
    return *res;
}

void CSpreadsheet::copyRect(const CPos &dst, const CPos &src, const int w, const int h) {
    std::map<CPos, CCell> tmp;
    const size_t xOffset = dst.m_x - src.m_x;
    const size_t yOffset = dst.m_y - src.m_y;
    m_eraseRun++;

    for (size_t x = 0; x < static_cast<size_t>(w); x++) {
        for (size_t y = 0; y < static_cast<size_t>(h); y++) {
            auto tmpDst = src.relativeMove(x, y);
            const auto it = m_sheet.lower_bound(tmpDst);
            if (it == m_sheet.end() || it->first != tmpDst)
                continue;
            tmp.emplace(*it);
        }
    }
    for (size_t x = 0; x < static_cast<size_t>(w); x++) {
        for (size_t y = 0; y < static_cast<size_t>(h); y++) {
            CPos ofsDst = src.relativeMove(x + xOffset, y + yOffset);
            const auto tmpIt = tmp.find(src.relativeMove(x, y));
            if (tmpIt == tmp.end()) {
                eraseCell(ofsDst);
            } else {
                CCell &cell = tmpIt->second;
                cell.moveReferences(xOffset, yOffset);
                setCell(ofsDst, cell);
            }
        }
    }
}
