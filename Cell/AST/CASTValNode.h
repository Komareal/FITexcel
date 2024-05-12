#ifndef CASTVALNODE_H
#define CASTVALNODE_H
#include "AASTNode.h"


class CASTValNode final : public AASTNode {
public:
    explicit CASTValNode(const CValue &val);

    CASTValNode(const CASTValNode &other);

    CASTValNode(CASTValNode &&other) noexcept;

    CASTValNode &operator=(CASTValNode other);

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager & /* unused_param */) const override;

    bool isVal() const override {
        return true;
    }

private:
    CSharedVal m_val;
};


#endif //CASTVALNODE_H
