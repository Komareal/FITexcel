#ifndef CASTOPNODE_H
#define CASTOPNODE_H


#include "CASTNodePtr.h"
#include "../../header.h"

enum class EBinaryType {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE
};

static std::unordered_map<EBinaryType, std::string> m_stringMap = {
    {EBinaryType::ADD, "+"},
    {EBinaryType::SUB, "-"},
    {EBinaryType::MUL, "*"},
    {EBinaryType::DIV, "/"},
    {EBinaryType::POW, "^"},
    {EBinaryType::EQ, "="},
    {EBinaryType::NE, "<>"},
    {EBinaryType::LT, "<"},
    {EBinaryType::LE, "<="},
    {EBinaryType::GT, ">"},
    {EBinaryType::GE, ">="}
};;

class CASTBinaryNode final : public AASTNode {
public:
    CASTBinaryNode(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), EBinaryType type);

    CASTBinaryNode(const CASTBinaryNode &other);

    CASTBinaryNode(CASTBinaryNode &&other) noexcept;

    CASTBinaryNode &operator=(CASTBinaryNode other);

    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    CSharedVal computeVal(CRefManager &refManager) const override;

    void addChild(const CASTNodePtr &child);

    bool hasValChildern() const;

private:
    CASTNodePtr m_childern[2];
    size_t m_childernSize;

    EBinaryType m_type;

    CSharedVal (*m_opFnc)(const CSharedVal &, const CSharedVal &);
};

#endif //CASTOPNODE_H
