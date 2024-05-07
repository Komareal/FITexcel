#include "CASTValNode.h"

CASTValNode::CASTValNode(const CValue &val) : m_val(std::make_shared<CValue>(val)) {
}

CASTValNode::CASTValNode(const CASTValNode &other): m_val(other.m_val) {
}

CASTValNode::CASTValNode(CASTValNode &&other) noexcept: m_val(std::move(other.m_val)) {
}

CASTValNode &CASTValNode::operator=(CASTValNode other) {
    std::swap(other.m_val, m_val);
    return *this;
}

void CASTValNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    using namespace std;
    if (holds_alternative<double>(*m_val))
        ss << get<double>(*m_val);
    else if (holds_alternative<string>(*m_val))
        ss << get<string>(*m_val);
}

CSharedVal CASTValNode::computeVal(CRefManager &refManager) const {
    return m_val;
}
