#ifndef CASTNODE_H
#define CASTNODE_H
#include "../header.h"


class CCell;
class CPos;
#ifndef __PROGTEST__
using CValue = std::variant<std::monostate, double, std::string>;
#endif
using CSharedVal = std::shared_ptr<CValue>;

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

//  ------------                        VAL                 OP       FUNC       REF          RANGE
using CASTNodeVal = std::variant<CSharedVal, EOpType, EFuncType, size_t, std::pair<size_t, size_t> >;

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
};


#endif //CASTNODE_H
