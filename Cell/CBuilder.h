#ifndef CBUILDER_H
#define CBUILDER_H
#include "../header.h"

#include "CCell.h"
#include "AST/CASTBinaryNode.h"
#include "AST/CASTUnaryNode.h"
#ifndef __PROGTEST__
#include "expression.h"
#endif

class CBuilder final : public CExprBuilder {
public:
    /**
     *
     * @param m_ref_manager reference manager
     */
    explicit CBuilder(CRefManager &m_ref_manager);

    /**
     *  Returns the root of the AST, throws exception if the stack bigger than 1
     * @return root of the AST
     */
    std::shared_ptr<AASTNode> getRoot();

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opAdd() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opSub() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opMul() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opDiv() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opPow() override;

    /**
     *  Calls the make bin function that will make the unary operation node
     */
    void opNeg() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opEq() override;

    /**
    *  Calls the make bin function that will make the binary operation node
    */
    void opNe() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opLt() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opLe() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opGt() override;

    /**
     *  Calls the make bin function that will make the binary operation node
     */
    void opGe() override;

    /**
     *  Makes a value node and pushes it to the stack
     * @param val  value of the node
     */
    void valNumber(double val) override;

    /**
     *  Makes a value node and pushes it to the stack
     * @param val  value of the node
     */
    void valString(std::string val) override;

    /**
     *  Makes a reference node and pushes it to the stack
     *   Adds the reference to the reference manager
     * @param val  Reference to the node like A1
     */
    void valReference(std::string val) override;

    /**
     *  Makes a range node and pushes it to the stack
     *   Adds both` references to the reference manager
     * @param val  References to the node like A1:B2
     */
    void valRange(std::string val) override;

    /**
     *  takes paramCount of items from the stack and makes a function node and pushes it back to the stack
     * @param fnName
     * @param paramCount
     */
    void funcCall(std::string fnName, int paramCount) override;

private:
    /**
     *  Helper stack for building the AST
     */
    std::stack<std::shared_ptr<AASTNode> > m_stack;

    /**
     *  Reference manager
     */
    CRefManager &m_refManager;

    /**
     * Takes two values from the stack and makes a binary operation node and pushes it back to the stack
     *  If the both next nodes in the stack are value nodes, it precomputes the value instead of making Binary operation the node
     * @param op_fnc  ptr to the function that will be used for the operation
     * @param type Enum of the operation type
     */
    void makeBinOp(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), EBinaryType type);

    /**
     * Takes one value from the stack and makes a unary operation node and pushes it back to the stack
     *  If the next node in the stack is a value node, it precomputes the value instead of making Unary operation the node
     * @param op_fnc ptr to the function that will be used for the operation
     * @param type  Enum of the operation type
     */
    void makeUnOp(CSharedVal (*op_fnc)(const CSharedVal &), CASTUnaryNode::EUnaryType type);
};


#endif //CBUILDER_H
