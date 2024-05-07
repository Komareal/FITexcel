#ifndef CCELL_H
#define CCELL_H
#include "../header.h"

#include "./AST/AASTNode.h"
#include "CRefManager.h"
#include "AST/CASTNodePtr.h"

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

    CSharedVal getValue(size_t run);

private:
    CSharedVal m_computedValue;
    size_t m_valueValidAt;
    CRefManager m_refManager;
    ECellState m_state;
    CASTNodePtr m_root;

    void saveVal(const CSharedVal &val);
};


#endif //CCELL_H
