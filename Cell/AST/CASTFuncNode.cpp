#include "CASTFuncNode.h"

#include "CASTOperations.h"
#include "../CPosRangeIterator.h"
#include "../CRefManager.h"


CASTFuncNode::CASTFuncNode(const std::string &type, const std::vector<std::shared_ptr<AASTNode>> &children)
    : m_children(children),
      m_type(EFuncType::SUM) {
    for (const auto &[key, value]: funcTypeStringMap) {
        if (value == type) {
            m_type = key;
            break;
        }
    }
}

CASTFuncNode::CASTFuncNode(const CASTFuncNode &other)
    : m_children(other.m_children),
      m_type(other.m_type) {
}

CASTFuncNode::CASTFuncNode(CASTFuncNode &&other) noexcept
    : m_children(std::move(other.m_children)),
      m_type(std::move(other.m_type)) {
}

CASTFuncNode &CASTFuncNode::operator=(CASTFuncNode other) {
    using namespace std;
    swap(m_type, other.m_type);
    swap(m_children, other.m_children);
    return *this;
}

void CASTFuncNode::print(std::stringstream &ss, const CRefManager &refManager) const {
    ss << funcTypeStringMap.find(m_type)->second << "( ";
    for (size_t i = m_children.size(); i > 0; --i) {
        m_children[i - 1]->print(ss, refManager);
        if (i != 1)
            ss << ", ";
    }
    ss << " )";
}

CSharedVal CASTFuncNode::computeVal(CRefManager &refManager) const {
    if (m_children.size() < 1) {
        return nullptr;
    }
    if (m_type == EFuncType::IF) {
        return computeIf(refManager);
    }

    const auto optionalIt = m_children[0]->getRangeIterator(refManager);
    if (!optionalIt.has_value()) {
        return nullptr;
    }

    auto it = optionalIt.value();

    CSharedVal res = nullptr;
    CSharedVal extraVal = nullptr;
    if (m_type == EFuncType::CNTVAL) {
        if (m_children.size() < 2) {
            return nullptr;
        }
        extraVal = m_children[1]->computeVal(refManager);
        if (extraVal == nullptr)
            return nullptr;
    }
    if (m_type == EFuncType::CNT || m_type == EFuncType::CNTVAL) {
        res = std::make_shared<CValue>(0.);
    }
    for (; !it.isEnd(); ++it) {
        switch (m_type) {
            case EFuncType::SUM:
                CASTOperations::sum(res, refManager.getValOfReference(it.get()));
                break;
            case EFuncType::CNT:
                CASTOperations::cnt(res, refManager.getValOfReference(it.get()));
                break;
            case EFuncType::MIN:
                CASTOperations::min(res, refManager.getValOfReference(it.get()));
                break;
            case EFuncType::MAX:
                CASTOperations::max(res, refManager.getValOfReference(it.get()));
                break;
            case EFuncType::CNTVAL:
                CASTOperations::cntVal(res, refManager.getValOfReference(it.get()), extraVal);
                break;
            default:
                break;
        }
    }
    return res;
}

CSharedVal CASTFuncNode::computeIf(CRefManager &refManager) const {
    if (m_children.size() < 3) {
        return nullptr;
    }
    CSharedVal condition = m_children[2]->computeVal(refManager);

    if (condition == nullptr || !holds_alternative<double>(*condition)) {
        return nullptr;
    }

    if (std::get<double>(*condition) == 0) {
        return m_children[0]->computeVal(refManager);
    }
    return m_children[1]->computeVal(refManager);
}
