#include "CBuilder.h"

#include "AST/CASTBinaryNode.h"
#include "AST/CASTOperations.h"
#include "AST/CASTRangeNode.h"
#include "AST/CASTRefNode.h"
#include "AST/CASTUnaryNode.h"
#include "AST/CASTValNode.h"
#include "AST/CASTFuncNode.h"


CBuilder::CBuilder(CRefManager &m_ref_manager)
    : m_refManager(m_ref_manager) {
}

CASTNodePtr CBuilder::getRoot() {
    if (m_stack.size() > 1)
        throw std::length_error("More nodes in stack");
    return std::move(m_stack.top());
}


void CBuilder::opAdd() {
    makeBinOp(&CASTOperations::add, EBinaryType::ADD);
}

void CBuilder::opSub() {
    makeBinOp(&CASTOperations::sub, EBinaryType::SUB);
}

void CBuilder::opMul() {
    makeBinOp(&CASTOperations::mul, EBinaryType::MUL);
}

void CBuilder::opDiv() {
    makeBinOp(&CASTOperations::div, EBinaryType::DIV);
}

void CBuilder::opPow() {
    makeBinOp(&CASTOperations::pow, EBinaryType::POW);
}

void CBuilder::opNeg() {
    makeUnOp(&CASTOperations::negate, CASTUnaryNode::EUnaryType::NEG);
}

void CBuilder::opEq() {
    makeBinOp(&CASTOperations::eq, EBinaryType::EQ);
}

void CBuilder::opNe() {
    makeBinOp(&CASTOperations::ne, EBinaryType::NE);
}

void CBuilder::opLt() {
    makeBinOp(&CASTOperations::lt, EBinaryType::LT);
}

void CBuilder::opLe() {
    makeBinOp(&CASTOperations::le, EBinaryType::LE);
}

void CBuilder::opGt() {
    makeBinOp(&CASTOperations::gt, EBinaryType::GT);
}

void CBuilder::opGe() {
    makeBinOp(&CASTOperations::ge, EBinaryType::GE);
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
    size_t index = 0;
    const CPos from(val, true, index);
    const CPos to(val, true, ++index);
    m_stack.emplace(
        std::make_shared<CASTRangeNode>(
            m_refManager.addReference(from),
            m_refManager.addReference(to)
        )
    );
}

void CBuilder::funcCall(const std::string fnName, const int paramCount) {
    if (m_stack.size() < paramCount)
        throw std::out_of_range("When creating function call not enough values are left");

    std::vector<CASTNodePtr> children;
    for (int i = 0; i < paramCount; i++) {
        children.push_back(m_stack.top());
        m_stack.pop();
    }
    auto node = CASTFuncNode(
        fnName, children
    );
    m_stack.emplace(std::make_shared<CASTFuncNode>(node));
}

void CBuilder::makeBinOp(CSharedVal (*op_fnc)(const CSharedVal &, const CSharedVal &), const EBinaryType type) {
    CASTBinaryNode node(op_fnc, type);
    if (m_stack.size() < 2)
        throw std::out_of_range("When creating binary operation not enough values are left");

    node.addChild(m_stack.top());
    m_stack.pop();
    node.addChild(m_stack.top());
    m_stack.pop();

    if (node.hasValChildern()) {
        const CSharedVal sharedRes = node.computeVal(m_refManager);
        m_stack.emplace(std::make_shared<CASTValNode>(sharedRes == nullptr ? CValue() : *sharedRes));
        return;
    }
    m_stack.emplace(std::make_shared<CASTBinaryNode>(std::move(node)));
}

void CBuilder::makeUnOp(CSharedVal (*op_fnc)(const CSharedVal &), const CASTUnaryNode::EUnaryType type) {
    if (m_stack.size() < 1)
        throw std::out_of_range("When creating unary operation none value is left");
    CASTUnaryNode tmp = {
        std::move(m_stack.top()),
        op_fnc,
        type
    };

    m_stack.pop();

    if (tmp.hasValChild()) {
        const CSharedVal sharedRes = tmp.computeVal(m_refManager);
        m_stack.emplace(std::make_shared<CASTValNode>(sharedRes == nullptr ? CValue() : *sharedRes));
        return;
    }

    m_stack.emplace(std::make_shared<CASTUnaryNode>(tmp));
}
