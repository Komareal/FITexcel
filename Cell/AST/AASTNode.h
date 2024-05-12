#ifndef AASTNode_H
#define AASTNode_H
#include "../../header.h"
#include "../CPosRangeIterator.h"

class CRefManager;
class CCell;
class CPos;


/**
 * Abstract class for AST nodes
 */
class AASTNode {
public:
    virtual ~AASTNode() = default;

    // ------------- Public interface
    /**
     *  Printing for load/save
     * @param ss stream to print to
     * @param refManager reference manager to resolve references
     */
    virtual void print(std::stringstream &ss, const CRefManager &refManager) const = 0;

    /**
     *  Recursively compute the value of the node
     * @param refManager reference manager to resolve references
     * @return shared pointer to the computed value
     */
    virtual CSharedVal computeVal(CRefManager &refManager) const = 0;


    /**
     * Cheks if the node is a value node
     * for the purpose of optimizing the tree
     * @return true if the node is a value node
     */
    virtual bool isVal() const {
        return false;
    };

    /**
     *  Get the range iterator of the range node
     * @return null if the node is not a range node or the iterator
     */
    virtual std::optional<CPosRangeIterator> getRangeIterator(CRefManager & /* unused_param */) const {
        return std::nullopt;
    };
};


#endif //AASTNode_H
