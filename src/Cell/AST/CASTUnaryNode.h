#ifndef CASTUNARYNODE_H
#define CASTUNARYNODE_H

#include "AASTNode.h"
#include "../../header.h"

/**
 *  Class for unary operation nodes
 */
class CASTUnaryNode final : public AASTNode {
public:
    /**
     *  Enum for the unary operation type (in progtest it is only NEG), but it is here for future expansion
     */
    enum class EUnaryType {
        NEG
    };

    CASTUnaryNode(std::shared_ptr<AASTNode> &&child, CSharedVal (*op_fnc)(const CSharedVal &), EUnaryType type);

    CASTUnaryNode(const CASTUnaryNode &other);

    CASTUnaryNode(CASTUnaryNode &&other) noexcept;

    CASTUnaryNode &operator=(CASTUnaryNode other);

    /**
     *
     * @return true if the node has a value child
     */
    bool hasValChild() const;

    /**
     *  Printing for load/save
     *  prints the operation and the child where the operation is prefixed
     * @param ss  stream to print to
     * @param refManager  reference manager of the cell to resolve references
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *  Recursively compute the value of the node (calls the operation function on the child)
     * @param refManager  reference manager of the cell to resolve references
     * @return  shared pointer to the computed value
     */
    CSharedVal computeVal(CRefManager &refManager) const override;

private:
    /**
     *  Child of the node
     */
    std::shared_ptr<AASTNode> m_child;

    /**
     *  Type of the operation
     */
    EUnaryType m_type;

    /**
     *  Function pointer to the operation function
     */
    CSharedVal (*m_opFnc)(const CSharedVal &);
};


#endif //CASTUNARYNODE_H
