//
// Created by ales on 5/4/24.
//

#ifndef CASTPROCCESSER_H
#define CASTPROCCESSER_H
#include "../header.h"

#include "CCell.h"

class CASTProccesser {
public:
    CASTProccesser(size_t setRun, size_t getRun, std::map<CPos, CCell> &map, CPosRefArr & refArr);

    CSharedVal getValue(const CASTNode &node);

private:
    const size_t m_setRun;
    const size_t m_getRun;
    std::map<CPos, CCell> &m_map;
    CPosRefArr & m_refArr;

    bool getBinaryOpValues(const CASTNode &node, CSharedVal & first, CSharedVal & second);

    bool getUnaryOpValue(const CASTNode &node,CSharedVal & val);

    CSharedVal add(const CASTNode &node);

    CSharedVal sub(const CASTNode &node);

    CSharedVal mul(const CASTNode &node);

    CSharedVal div(const CASTNode &node);

    CSharedVal pow(const CASTNode &node);

    CSharedVal neg(const CASTNode &node);

    CSharedVal eq(const CASTNode &node);

    CSharedVal lt(const CASTNode &node);

    CSharedVal le(const CASTNode &node);

    CSharedVal gt(const CASTNode &node);

    CSharedVal ge(const CASTNode &node);
};


#endif //CASTPROCCESSER_H
