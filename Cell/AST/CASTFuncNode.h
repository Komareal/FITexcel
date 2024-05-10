#ifndef CASTFUNCNODE_H
#define CASTFUNCNODE_H
#include "CASTNodePtr.h"
#include "CASTRangeNode.h"

enum class EFuncType {
    SUM,
    CNT,
    MIN,
    MAX,
    CNTVAL,
    IF
};

static std::unordered_map<EFuncType, std::string> funcTypeStringMap = {
    {EFuncType::SUM, "sum"},
    {EFuncType::CNT, "count"},
    {EFuncType::MIN, "min"},
    {EFuncType::MAX, "max"},
    {EFuncType::CNTVAL, "countval"},
    {EFuncType::IF, "if"}
};

class CASTFuncNode final : public AASTNode {
public:
    CASTFuncNode(const std::string &type, const std::vector<CASTNodePtr> &children);

    CASTFuncNode(const CASTFuncNode &other);

    CASTFuncNode(CASTFuncNode &&other) noexcept;

    CASTFuncNode &operator=(CASTFuncNode other);

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

    CSharedVal computeIf(CRefManager &refManager) const;

private:
    std::vector<CASTNodePtr> m_children;
    EFuncType m_type;
};


#endif //CASTFUNCNODE_H
