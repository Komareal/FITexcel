#ifndef CCELL_H
#define CCELL_H
#include "CPos.h"
#include "../header.h"
class CCell;
using CPosRefArr = std::vector<std::pair<CPos, CCell *> >;


#include "CASTNode.h"

/**
 *
 */
class CCell {
public:
    // ------------ Constructors
    CCell(std::string_view str);

    // ------------ Move and copy
    CCell(const CCell &other);

    CCell(CCell &&other) noexcept;

    CCell &operator=(CCell other);

    // ------------ Public interface
    CValue getVal(size_t run);

private:
    CValue m_computedValue;
    size_t m_computedAt;
    std::unique_ptr<CASTNode> m_root;
    CPosRefArr m_references;
};


#endif //CCELL_H
