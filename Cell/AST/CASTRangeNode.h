#ifndef CASTRANGENODE_H
#define CASTRANGENODE_H
#include "AASTNode.h"


class CASTRangeNode final : public AASTNode {
public:
    friend class CASTFuncNode;

    CASTRangeNode(size_t left_reference, size_t right_reference);

    CASTRangeNode(const CASTRangeNode &other);

    CASTRangeNode(CASTRangeNode &&other) noexcept;

    CASTRangeNode &operator=(CASTRangeNode other);

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

    std::optional<CPosRangeIterator> getRangeIterator(CRefManager &refManager) const override;

private:
    size_t m_leftReference;
    size_t m_rightReference;
};


#endif //CASTRANGENODE_H
