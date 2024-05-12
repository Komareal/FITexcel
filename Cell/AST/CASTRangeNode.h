#ifndef CASTRANGENODE_H
#define CASTRANGENODE_H
#include "AASTNode.h"


/**
 *  Node that represents a range of cells
 */
class CASTRangeNode final : public AASTNode {
public:
    friend class CASTFuncNode;

    /**
     *  Constructor
     * @param left_reference index of reference in refManager
     * @param right_reference index of reference in refManager
     */
    CASTRangeNode(size_t left_reference, size_t right_reference);

    CASTRangeNode(const CASTRangeNode &other);

    CASTRangeNode(CASTRangeNode &&other) noexcept;

    CASTRangeNode &operator=(CASTRangeNode other);

    /**
     *  Prints the range as A1:B2
     * @param ss 
     * @param refManager 
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *  Returns nullptr
     * @return  nullptr
     */
    CSharedVal computeVal(CRefManager & /* unused_param */) const override;

    /**
     *  Returns iterator for the range
     * @param refManager  reference manager from where to get the references
     * @return  iterator for the range
     */
    std::optional<CPosRangeIterator> getRangeIterator(CRefManager &refManager) const override;

private:
    /**
     *  Index of the left reference in refManager
     */
    size_t m_leftReference;

    /**
     *  Index of the right reference in refManager
     */
    size_t m_rightReference;
};


#endif //CASTRANGENODE_H
