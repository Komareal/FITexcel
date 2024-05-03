#include <utility>

#ifndef CASTNODE_H
#define CASTNODE_H

enum ENodeType {
    VALUE,
    UNARY,
    BINARY,
    TERNARY
};

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

//  ------------                    OP     REF          RANGE           VAL      STR VAL      FUNC
using CASTNodeVal = std::variant<EOpType, CPos, std::pair<CPos, CPos>, double, std::string, EFuncType>;

class CASTNode {
public:
    // ------------  Constructors
    CASTNode(ENodeType type, CASTNodeVal val);

    // ------------  Move & copy
    CASTNode(const CASTNode &other);

    CASTNode(CASTNode &&other) noexcept;

    CASTNode &operator=(CASTNode other);

    ENodeType m_type;
    CASTNodeVal m_val;
    std::unique_ptr<CASTNode> m_childern[3];

private:
};


#endif //CASTNODE_H
