#ifndef CASTREFNODE_H
#define CASTREFNODE_H
#include "AASTNode.h"


/**
 *  Node that represents a reference to a cell
 */
class CASTRefNode final : public AASTNode {
public:

 /**
     * Constructor
     * @param ref  index of reference in refManager
     */
    explicit CASTRefNode(size_t ref);

    CASTRefNode(const CASTRefNode &other);

    CASTRefNode(CASTRefNode &&other) noexcept;

    CASTRefNode &operator=(CASTRefNode other);

    /**
     * Prints the reference as A1
     * @param ss
     * @param refManager
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *  Returns value of the node that is referenced
     * @param refManager
     * @return  value of the  node that is referenced
     */
    CSharedVal computeVal(CRefManager &refManager) const override;

private:
    /**
     * Index of the reference in refManager
     */
    size_t m_ref;
};


#endif //CASTREFNODE_H
