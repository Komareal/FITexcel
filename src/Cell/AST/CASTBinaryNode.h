#ifndef CASTOPNODE_H
#define CASTOPNODE_H


#include "AASTNode.h"
#include "../../header.h"

/**
 *  Enum class for binary operations
 */
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

/**
 *  Map for string representation of binary operations
 */
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

/**
 *  Class for binary operations
 */
class CASTBinaryNode final : public AASTNode {
public:
    /**
     *  Constructor
     * @param op_fnc
     * @param type
     */
    CASTBinaryNode(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), EBinaryType type);

    /**
     *  Copy constructor
     * @param other
     */
    CASTBinaryNode(const CASTBinaryNode &other);

    /**
     *  Move constructor
     * @param other
     */
    CASTBinaryNode(CASTBinaryNode &&other) noexcept;

    /**
     *  operator=
     * @param other
     * @return
     */
    CASTBinaryNode &operator=(CASTBinaryNode other);

    /**
     *  print for load/save
     *  arg0 op arg1
     * @param ss
     * @param refManager
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *  compute value
     * @param refManager  reference manager to resolve references
     * @return  shared pointer to the computed value
     */
    CSharedVal computeVal(CRefManager &refManager) const override;

    /**
     *  Add child to the node
     * @param child
     */
    void addChild(const std::shared_ptr<AASTNode> &child);

    /**
     *  Check if the node has value children for optimization
     *  if the node has value children, it can be precomputed
     * @return  true if the node has value children
     */
    bool hasValChildern() const;

private:
    /**
     * Children of the node
     * in reverse order (because of stack) - 0 is the second child
     */
    std::shared_ptr<AASTNode> m_childern[2];

    /**
     *  Number of children
     */
    size_t m_childernSize;

    /**
     *  Type of the operation
     */
    EBinaryType m_type;

    /**
     *  Function ptr for the operation
     */
    CSharedVal (*m_opFnc)(const CSharedVal &, const CSharedVal &);
};

#endif //CASTOPNODE_H
