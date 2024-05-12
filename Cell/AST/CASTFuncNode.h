#ifndef CASTFUNCNODE_H
#define CASTFUNCNODE_H
#include "CASTRangeNode.h"

/**
 *  Enum class for function types
 */
enum class EFuncType {
    SUM,
    CNT,
    MIN,
    MAX,
    CNTVAL,
    IF
};

/**
 *  Map for string representation of function types
 */
static std::unordered_map<EFuncType, std::string> funcTypeStringMap = {
    {EFuncType::SUM, "sum"},
    {EFuncType::CNT, "count"},
    {EFuncType::MIN, "min"},
    {EFuncType::MAX, "max"},
    {EFuncType::CNTVAL, "countval"},
    {EFuncType::IF, "if"}
};

/**
 *  Class for function nodes
 */
class CASTFuncNode final : public AASTNode {
public:
    /**
     *  Constructor
     * @param type What function is this (makes a enum from string)
     * @param children  What are the arguments of the function
     */
    CASTFuncNode(const std::string &type, const std::vector<std::shared_ptr<AASTNode>> &children);

    /**
     * Copy constructor
     * @param other
     */
    CASTFuncNode(const CASTFuncNode &other);

    /**
     * Move constructor
     * @param other
     */
    CASTFuncNode(CASTFuncNode &&other) noexcept;

    CASTFuncNode &operator=(CASTFuncNode other);

    /**
     *  Prints name(arg0, arg1, ...) of the function
     * @param ss
     * @param refManager
     */
    void print(std::stringstream &ss, const CRefManager &refManager) const override;

    /**
     *  Computes the value of the function, if the funcion is if then it calls computeIf, else it iterates over the range and performs action
     * @param refManager  reference manager to resolve references
     * @return  shared pointer to the computed value
     */
    CSharedVal computeVal(CRefManager &refManager) const override;

    /**
     *  computes if function
     * @param refManager
     * @return  shared pointer to the computed value
     */
    CSharedVal computeIf(CRefManager &refManager) const;

private:
    /**
     *  Multifunction array for arguments
     */
    std::vector<std::shared_ptr<AASTNode>> m_children;
    /**
     *  Function type
     */
    EFuncType m_type;
};


#endif //CASTFUNCNODE_H
