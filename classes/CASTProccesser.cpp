//
// Created by ales on 5/4/24.
//

#include "CASTProccesser.h"

using std::holds_alternative, std::make_shared, std::string;

CASTProccesser::CASTProccesser(const size_t setRun, const size_t eraseRun, const size_t cellPtrValidAt, std::map<CPos, CCell> &map, CPosRefArr &refArr)
    : m_setRun(setRun),
      m_eraseRun(eraseRun),
      m_cellPtrValidAt(cellPtrValidAt),
      m_map(map),
      m_refArr(refArr) {
}

CSharedVal CASTProccesser::getValue(const CASTNode &node) {
    if (std::holds_alternative<CSharedVal>(node.m_val)) {
        return std::get<CSharedVal>(node.m_val);
    }
    if (std::holds_alternative<size_t>(node.m_val)) {
        auto [pos, ptr] = m_refArr[std::get<size_t>(node.m_val)];
        if (ptr == nullptr || m_eraseRun != m_cellPtrValidAt) {
            if (m_map.contains(pos)) {
                ptr = &m_map.at(pos);
                m_refArr[std::get<size_t>(node.m_val)] = std::pair(pos, ptr);
            } else {
                return nullptr;
            }
        }
        return ptr->getValue(m_setRun, m_eraseRun, m_map);
    }
    if (std::holds_alternative<EOpType>(node.m_val)) {
        switch (std::get<EOpType>(node.m_val)) {
            case ADD:
                return add(node);
            case SUB:
                return sub(node);
            case MUL:
                return mul(node);
            case DIV:
                return div(node);
            case POW:
                return pow(node);
            case NEG:
                return neg(node);
            case EQ:
                return eq(node);
            case NE:
                return neg(node);
            case LT:
                return lt(node);
            case LE:
                return le(node);
            case GT:
                return gt(node);
            case GE:
                return ge(node);
        }
    }
    // if (std::holds_alternative<EFuncType>(node.m_val)) {
    //     switch (std::get<EFuncType>(node.m_val)) {
    //         case SUM:
    //             break;
    //         case CNT:
    //             break;
    //         case MIN:
    //             break;
    //         case MAX:
    //             break;
    //         case CNTVAL:
    //             break;
    //         case IF:
    //             break;
    //     }
    // }
    return nullptr;
}

bool CASTProccesser::getBinaryOpValues(const CASTNode &node, CSharedVal &first, CSharedVal &second) {
    first = getValue(*node.m_childern[1]);
    second = getValue(*node.m_childern[0]);
    return first != nullptr && second != nullptr;
}

bool CASTProccesser::getUnaryOpValue(const CASTNode &node, CSharedVal &val) {
    val = getValue(*node.m_childern[0]);
    return val != nullptr;
}

CSharedVal CASTProccesser::add(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::get<double>(*first) + std::get<double>(*second));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(std::get<string>(*first) + std::get<string>(*second));
    }
    if (holds_alternative<double>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(std::to_string(std::get<double>(*first)) + std::get<string>(*second));
    }
    if (holds_alternative<string>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::get<string>(*first) + std::to_string(std::get<double>(*second)));
    }
    return nullptr;
}

CSharedVal CASTProccesser::sub(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::get<double>(*first) - std::get<double>(*second));
    }
    return nullptr;
}

CSharedVal CASTProccesser::mul(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::get<double>(*first) * std::get<double>(*second));
    }
    return nullptr;
}

CSharedVal CASTProccesser::div(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::get<double>(*first) / std::get<double>(*second));
    }
    return nullptr;
}

CSharedVal CASTProccesser::pow(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(std::pow(std::get<double>(*first), std::get<double>(*second)));
    }
    return nullptr;
}

CSharedVal CASTProccesser::neg(const CASTNode &node) {
    CSharedVal val;
    if (!getUnaryOpValue(node, val))
        return nullptr;
    if (holds_alternative<double>(*val))
        return make_shared<CValue>(-std::get<double>(*val));
    return nullptr;
}

CSharedVal CASTProccesser::eq(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<double>(*first) == std::get<double>(*second
                )
            ));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<string>(*first) == std::get<string>(*second
                )
            ));
    }
    return nullptr;
}

CSharedVal CASTProccesser::lt(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<double>(*first) < std::get<double>(*second
                )
            ));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<string>(*first) < std::get<string>(*second
                )
            ));
    }
    return nullptr;
}

CSharedVal CASTProccesser::le(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<double>(*first) <= std::get<double>(*second
                )
            ));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<string>(*first) <= std::get<string>(*second
                )
            ));
    }
    return nullptr;
}

CSharedVal CASTProccesser::gt(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<double>(*first) > std::get<double>(*second
                )
            ));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<string>(*first) > std::get<string>(*second
                )
            ));
    }
    return nullptr;
}

CSharedVal CASTProccesser::ge(const CASTNode &node) {
    CSharedVal first, second;
    if (!getBinaryOpValues(node, first, second))
        return nullptr;
    if (holds_alternative<double>(*first) && holds_alternative<double>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<double>(*first) >= std::get<double>(*second
                )
            ));
    }
    if (holds_alternative<string>(*first) && holds_alternative<string>(*second)) {
        return make_shared<CValue>(
            static_cast<double>(
                std::get<string>(*first) >= std::get<string>(*second
                )
            ));
    }
    return nullptr;
}
