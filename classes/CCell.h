#ifndef CCELL_H
#define CCELL_H
#include "../header.h"
#include "CPos.h"
#include "CASTNode.h"

class CCell;
using CPosRefArr = std::vector<std::pair<CPos, CCell *> >;

/**
 *
 */
class CCell {
public:
    friend class CSpreadsheet;
    enum class ECellState {
        FRESH,
        OPEN,
        CLOSED,
    };

    // ------------ Constructors
    CCell(std::string_view str);

    // ------------ Move and copy
    CCell(const CCell &other);

    CCell(CCell &&other) noexcept;

    CCell &operator=(CCell other);

    CSharedVal getValue(size_t setRun, size_t getRun, std::map<CPos, CCell> &map);

private:
   CSharedVal m_computedValue;
    size_t m_valueValidAt;
    size_t m_stateGivenAt;
    ECellState m_state;
    std::unique_ptr<CASTNode> m_root;
    CPosRefArr m_references;

    void saveVal(const CSharedVal &val, size_t setRun);

};


#endif //CCELL_H
