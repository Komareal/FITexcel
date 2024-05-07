#ifndef CASTREFNODE_H
#define CASTREFNODE_H
#include "AASTNode.h"


class CASTRefNode final : public AASTNode {
public:
    explicit CASTRefNode(size_t ref);

    CASTRefNode(const CASTRefNode &other);

    CASTRefNode(CASTRefNode &&other) noexcept;

    CASTRefNode &operator=(CASTRefNode other);

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

private:
    size_t m_ref;
};


#endif //CASTREFNODE_H
