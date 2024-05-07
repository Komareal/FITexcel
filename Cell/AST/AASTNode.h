#ifndef AASTNode_H
#define AASTNode_H
#include "../CRefManager.h"
#include "../../header.h"


class CCell;
class CPos;


class AASTNode {
public:
    virtual ~AASTNode() = default;

    // ------------- Public interface
    virtual void print(std::stringstream &ss, const CRefManager &refManager) const = 0;

    virtual CSharedVal computeVal(CRefManager &refManager) const = 0;

    virtual bool isVal() const {
        return false;
    };
};


#endif //AASTNode_H
