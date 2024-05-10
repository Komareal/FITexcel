#ifndef CREFMANAGER_H
#define CREFMANAGER_H
#include "../header.h"
#include "CPos.h"

class CCell;
class CSpreadsheet;
using CRefCacheArr = std::vector<std::pair<CPos, CCell *> >;

class CRefManager {
public:
    friend class CSpreadsheet;

    CRefManager();

    CRefManager(const CRefManager &other);

    CRefManager(CRefManager &&other) noexcept;

    CRefManager &operator=(CRefManager other);

    size_t addReference(const CPos &pos);

    CSharedVal getValOfReference(size_t i);

    CSharedVal getValOfReference(const CPos &pos) const;

    CPos getPosOfReference(size_t i);

    void print(std::ostream &os, size_t i) const;

    void moveReferences(size_t x, size_t y);

private:
    CSpreadsheet *m_base;
    size_t m_validAt;
    CRefCacheArr m_references;

    CCell *getReference(size_t i);
};


#endif //CREFMANAGER_H
