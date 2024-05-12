#ifndef CCELL_H
#define CCELL_H
#include "../header.h"

#include "./AST/AASTNode.h"
#include "CRefManager.h"

/**
 * Class for cells in the spreadsheet
 */
class CCell {
public:
    friend class CSpreadsheet;

    /**
     *  Enum class for cell states for cycle detection
     */
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
    /**
     * Cached value of the cell
     */
    CSharedVal m_computedValue;

    /**
     *  Value cached at this run
     */
    size_t m_valueValidAt;


    /**
     *  Reference manager for the cell
     */
    CRefManager m_refManager;


    /**
     *  State of the cell for cycle detection (is valid same as m_computedValue)
     */
    ECellState m_state;

    /**
     *  Root of the AST
     */
    std::shared_ptr<AASTNode> m_root;

    /**
     *  Saves the value of the cell and sets the state to closed
     * @param val 
     */
    void saveVal(const CSharedVal &val);
};


#endif //CCELL_H
