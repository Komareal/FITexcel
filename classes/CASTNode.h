#ifndef CASTNODE_H
#define CASTNODE_H
#include "../header.h"


using CValue = std::variant<std::monostate, double, std::string>;

enum EOpType {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    NEG,
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE
};

enum EFuncType {
    SUM,
    CNT,
    MIN,
    MAX,
    CNTVAL,
    IF
};

//  ------------                    OP      REF          RANGE                VAL      STR VAL      FUNC
using CASTNodeVal = std::variant<EOpType, size_t, std::pair<size_t, size_t>, double, std::string, EFuncType>;

class CASTNode {
public:
    // ------------  Constructors
    CASTNode(CASTNodeVal val);

    // ------------  Move & copy
    CASTNode(const CASTNode &other);

    CASTNode(CASTNode &&other) noexcept;

    CASTNode &operator=(CASTNode other);

    CASTNodeVal m_val;
    std::unique_ptr<CASTNode> m_childern[3];
    size_t m_childernSize;

private:
};


#endif //CASTNODE_H
