#include "CBuilder.h"

#include "AST/CASTBinaryNode.h"
#include "AST/CASTOperations.h"
#include "AST/CASTRefNode.h"
#include "AST/CASTUnaryNode.h"
#include "AST/CASTValNode.h"


CBuilder::CBuilder(CRefManager &m_ref_manager)
    : m_refManager(m_ref_manager) {
}

CASTNodePtr CBuilder::getRoot() {
    if (m_stack.size() > 1)
        throw std::length_error("More nodes in stack");
    return std::move(m_stack.top());
}


void CBuilder::opAdd() {
    makeBinOp(&CASTOperations::add, CASTBinaryNode::EBinaryType::ADD);
}

void CBuilder::opSub() {
    makeBinOp(&CASTOperations::sub, CASTBinaryNode::EBinaryType::SUB);
}

void CBuilder::opMul() {
    makeBinOp(&CASTOperations::mul, CASTBinaryNode::EBinaryType::MUL);
}

void CBuilder::opDiv() {
    makeBinOp(&CASTOperations::div, CASTBinaryNode::EBinaryType::DIV);
}

void CBuilder::opPow() {
    makeBinOp(&CASTOperations::pow, CASTBinaryNode::EBinaryType::POW);
}

void CBuilder::opNeg() {
    makeUnOp(&CASTOperations::negate, CASTUnaryNode::EUnaryType::NEG);
}

void CBuilder::opEq() {
    makeBinOp(&CASTOperations::eq, CASTBinaryNode::EBinaryType::EQ);
}

void CBuilder::opNe() {
    makeBinOp(&CASTOperations::ne, CASTBinaryNode::EBinaryType::NE);
}

void CBuilder::opLt() {
    makeBinOp(&CASTOperations::lt, CASTBinaryNode::EBinaryType::LT);
}

void CBuilder::opLe() {
    makeBinOp(&CASTOperations::le, CASTBinaryNode::EBinaryType::LE);
}

void CBuilder::opGt() {
    makeBinOp(&CASTOperations::gt, CASTBinaryNode::EBinaryType::GT);
}

void CBuilder::opGe() {
    makeBinOp(&CASTOperations::ge, CASTBinaryNode::EBinaryType::GE);
}

void CBuilder::valNumber(double val) {
    m_stack.emplace(std::make_shared<CASTValNode>(val));
}

void CBuilder::valString(std::string val) {
    m_stack.emplace(std::make_shared<CASTValNode>(val));
}

void CBuilder::valReference(const std::string val) {
    const CPos ref(val, false);
    m_stack.emplace(
        std::make_shared<CASTRefNode>(
            m_refManager.addReference(ref)
        )
    );
}

void CBuilder::valRange(const std::string val) {
    // size_t index = 0;
    // const CPos from(val, true, index);
    // const CPos to(val, true, ++index);
    // m_stack.emplace(
    //     std::pair(
    //         emplaceRef(from),
    //         emplaceRef(to)
    //     )
    // );
}

void CBuilder::funcCall(const std::string fnName, const int paramCount) {
    // // Just so compiler shuts up
    // if (fnName == "sum")
    //     if (paramCount == 0)
    //         m_stack.emplace(EFuncType::SUM);
}

void CBuilder::makeBinOp(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), const CASTBinaryNode::EBinaryType type) {
    CASTBinaryNode node(op_fnc, type);
    if (m_stack.size() < 2)
        throw std::out_of_range("When creating binary operation not enough values are left");

    node.addChild(m_stack.top());
    m_stack.pop();
    node.addChild(m_stack.top());
    m_stack.pop();

    m_stack.emplace(std::make_shared<CASTBinaryNode>(std::move(node)));
}

void CBuilder::makeUnOp(CSharedVal (*op_fnc)(const CSharedVal &), CASTUnaryNode::EUnaryType type) {
    if (m_stack.size() < 1)
        throw std::out_of_range("When creating unary operation none value is left");
    CASTNodePtr tmp = CASTNodePtr(
        std::make_shared<CASTUnaryNode>(
            std::move(m_stack.top()),
            op_fnc,
            type
        )
    );
    m_stack.pop();
    m_stack.emplace(tmp);
}
