#ifndef CBUILDER_H
#define CBUILDER_H
#include "../header.h"

#include "CCell.h"
#include "AST/CASTBinaryNode.h"
#include "AST/CASTNodePtr.h"
#include "AST/CASTUnaryNode.h"
#ifndef __PROGTEST__
#include "expression.h"
#endif

class CBuilder : public CExprBuilder {
public:
    explicit CBuilder(CRefManager &m_ref_manager);

    CASTNodePtr getRoot();


    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(std::string val) override;

    void valReference(std::string val) override;

    void valRange(std::string val) override;

    void funcCall(std::string fnName, int paramCount) override;

private:
    std::stack<CASTNodePtr> m_stack;
    CRefManager &m_refManager;

    void makeBinOp(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), EBinaryType type);

    void makeUnOp(CSharedVal (*op_fnc)(const CSharedVal &), CASTUnaryNode::EUnaryType type);
};


#endif //CBUILDER_H
