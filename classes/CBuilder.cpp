#include "CBuilder.h"

#include <memory>

CBuilder::CBuilder(const std::string_view &str, CPosRefArr &references)
    : m_str(str),
      m_references(references) {
}

std::unique_ptr<CASTNode> CBuilder::getRoot() {
    if (m_stack.size() > 1)
        throw std::length_error("More nodes in stack");
    return std::make_unique<CASTNode>(m_stack.top());
}

void CBuilder::makeOp(EOpType type, const size_t childnum) {
    CASTNode node(type);
    for (size_t i = 0; i < childnum; ++i) {
        node.m_childern[i] = std::make_unique<CASTNode>(m_stack.top());
        m_stack.pop();
    }
    node.m_childernSize = childnum;
    m_stack.emplace(node);
}

size_t CBuilder::emplaceRef(const CPos &pos) const {
    for (size_t i = 0; i < m_references.size(); i++)
        if (m_references[i].first.equalsWithFix(pos))
            return i;

    m_references.emplace_back(pos, nullptr);
    return m_references.size() - 1;
}

void CBuilder::opAdd() {
    makeOp(EOpType::ADD, 2);
}

void CBuilder::opSub() {
    makeOp(EOpType::SUB, 2);
}

void CBuilder::opMul() {
    makeOp(EOpType::MUL, 2);
}

void CBuilder::opDiv() {
    makeOp(EOpType::DIV, 2);
}

void CBuilder::opPow() {
    makeOp(EOpType::POW, 2);
}

void CBuilder::opNeg() {
    makeOp(EOpType::NEG, 1);
}

void CBuilder::opEq() {
    makeOp(EOpType::EQ, 2);
}

void CBuilder::opNe() {
    makeOp(EOpType::NE, 2);
}

void CBuilder::opLt() {
    makeOp(EOpType::LT, 2);
}

void CBuilder::opLe() {
    makeOp(EOpType::LE, 2);
}

void CBuilder::opGt() {
    makeOp(EOpType::GT, 2);
}

void CBuilder::opGe() {
    makeOp(EOpType::GE, 2);
}

void CBuilder::valNumber(double val) {
    m_stack.emplace(std::make_shared<CValue>(val));
}

void CBuilder::valString(std::string val) {
    m_stack.emplace(std::make_shared<CValue>(val));
}

void CBuilder::valReference(const std::string val) {
    const CPos ref(val, false);
    m_stack.emplace(emplaceRef(ref));
}

void CBuilder::valRange(const std::string val) {
    size_t index = 0;
    const CPos from(val, true, index);
    const CPos to(val, true, ++index);
    m_stack.emplace(
        std::pair(
            emplaceRef(from),
            emplaceRef(to)
        )
    );
}

void CBuilder::funcCall(const std::string fnName, const int paramCount) {
    // Just so compiler shuts up
    if (fnName == "sum")
        if (paramCount == 0)
            m_stack.emplace(EFuncType::SUM);
}
