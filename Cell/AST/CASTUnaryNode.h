#ifndef CASTUNARYNODE_H
#define CASTUNARYNODE_H

#include "CASTNodePtr.h"
#include "../../header.h"

class CASTUnaryNode final : public AASTNode {
public:
    enum class EUnaryType {
        NEG
    };

    CASTUnaryNode(CASTNodePtr &&child, CSharedVal (*op_fnc)(const CSharedVal &), EUnaryType type);

    CASTUnaryNode(const CASTUnaryNode &other);

    CASTUnaryNode(CASTUnaryNode &&other) noexcept;

    CASTUnaryNode &operator=(CASTUnaryNode other);

private:
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

    CASTNodePtr m_child;

    EUnaryType m_type;

    CSharedVal (*m_opFnc)(const CSharedVal &);
};


#endif //CASTUNARYNODE_H
