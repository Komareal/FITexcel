

#ifndef CASTFUNCNODE_H
#define CASTFUNCNODE_H
#include "AASTNode.h"

class CASTFuncNode : public AASTNode {
    enum class EFuncType {
        SUM,
        CNT,
        MIN,
        MAX,
        CNTVAL,
        IF
    };
};


#endif //CASTFUNCNODE_H
