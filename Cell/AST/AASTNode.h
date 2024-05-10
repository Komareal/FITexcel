#ifndef AASTNode_H
#define AASTNode_H
#include "../../header.h"
#include "../CPosRangeIterator.h"

class CRefManager;
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

    virtual std::optional<CPosRangeIterator> getRangeIterator(CRefManager &refManager) const {
        return std::nullopt;
    };
};


#endif //AASTNode_H
