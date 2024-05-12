#ifndef CREFMANAGER_H
#define CREFMANAGER_H
#include "../header.h"
#include "CPos.h"

class CCell;
class CSpreadsheet;
using CRefCacheArr = std::vector<std::pair<CPos, CCell *> >;

/**
 *  Class for managing references to the cells in the spreadsheet in the AST
 */
class CRefManager {
public:
    friend class CSpreadsheet;

    CRefManager();

    CRefManager(const CRefManager &other);

    CRefManager(CRefManager &&other) noexcept;

    CRefManager &operator=(CRefManager other);

    /**
     *  Used in builder to add reference to the cell
     * @param pos position of the cell
     * @return index of the reference to save in the AST
     */
    size_t addReference(const CPos &pos);

    /**
     *
     * @param i index of the reference
     * @return  value of the Cell that is referenced
     */
    CSharedVal getValOfReference(size_t i);

    /**
     *
     * @param pos  position of the cell
     * @return value of the Cell on the position
     */
    CSharedVal getValOfReference(const CPos &pos) const;

    /**
     *  Used for range nodes to get the iterator for the range
     * @param i index of the reference
     * @return  position of the Cell that is referenced
     */
    CPos getPosOfReference(size_t i);

    /**
     *  Prints the reference on i as A1
     * @param os
     * @param i
     */
    void print(std::ostream &os, size_t i) const;

    /**
     *  Moves all references in the cache by x and y
     * @param x
     * @param y
     */
    void moveReferences(size_t x, size_t y);

private:
    /**
     *  Base spreadsheet
     */
    CSpreadsheet *m_base;

    /**
     *  when the pointer cache are valid
     */
    size_t m_validAt;

    /**
     *  Array of pairs of positions and pointers to the cells for optimization
     *  the exact pointers are used when no cell was erased since when the cache was valid
     */
    CRefCacheArr m_references;

    CCell *getReference(size_t i);
};


#endif //CREFMANAGER_H
