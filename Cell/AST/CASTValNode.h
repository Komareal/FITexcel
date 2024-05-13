#ifndef CASTVALNODE_H
#define CASTVALNODE_H
#include "AASTNode.h"


/**
 *  Class for value nodes
 */
class CASTValNode final : public AASTNode {
public:
    explicit CASTValNode(const CValue &val);

    CASTValNode(const CASTValNode &other);

    CASTValNode(CASTValNode &&other) noexcept;

    CASTValNode &operator=(CASTValNode other);

    /**
     *  Printing for load/save
     *  prints the value
     * @param ss  stream to print to
     * @param refManager  reference manager to resolve references
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *
     * @return the value of the node
     */
    CSharedVal computeVal(CRefManager & /* unused_param */) const override;

    /**
     *  Returns true as the node is a value node
     * @return true
     */
    bool isVal() const override {
        return true;
    }

private:
    /**
     *  Value of the node
     */
    CSharedVal m_val;
};


#endif //CASTVALNODE_H
