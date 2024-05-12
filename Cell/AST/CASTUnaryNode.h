#ifndef CASTUNARYNODE_H
#define CASTUNARYNODE_H

#include "AASTNode.h"
#include "../../header.h"

class CASTUnaryNode final : public AASTNode {
public:
    enum class EUnaryType {
        NEG
    };

    CASTUnaryNode(std::shared_ptr<AASTNode> &&child, CSharedVal (*op_fnc)(const CSharedVal &), EUnaryType type);

    CASTUnaryNode(const CASTUnaryNode &other);

    CASTUnaryNode(CASTUnaryNode &&other) noexcept;

    CASTUnaryNode &operator=(CASTUnaryNode other);

    bool hasValChild() const;

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

private:
    std::shared_ptr<AASTNode> m_child;

    EUnaryType m_type;

    CSharedVal (*m_opFnc)(const CSharedVal &);
};


#endif //CASTUNARYNODE_H
