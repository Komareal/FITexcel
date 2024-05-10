#ifndef CASTOPERATIONS_H
#define CASTOPERATIONS_H
#include "../../header.h"


class CASTOperations {
public:
    static CSharedVal add(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal sub(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal mul(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal div(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal pow(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal negate(const CSharedVal &val);

    static CSharedVal eq(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal ne(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal lt(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal le(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal gt(const CSharedVal &first, const CSharedVal &second);

    static CSharedVal ge(const CSharedVal &first, const CSharedVal &second);

    static void sum(CSharedVal &carry, const CSharedVal &val);

    static void cnt(const CSharedVal &carry, const CSharedVal &val);

    static void min(CSharedVal &carry, const CSharedVal &val);

    static void max(CSharedVal &carry, const CSharedVal &val);

    static void cntVal(const CSharedVal &carry, const CSharedVal &val, const CSharedVal &lookFor);

private:
    static bool assumeDoubles(const CSharedVal &first, const CSharedVal &second);

    static bool assumeStrings(const CSharedVal &first, const CSharedVal &second);

    static bool assumeSD(const CSharedVal &first, const CSharedVal &second);

    static bool assumeDS(const CSharedVal &first, const CSharedVal &second);

    static double convertBoolToDouble(bool x);

    static CSharedVal negateBool(const CSharedVal &val);
};


#endif //CASTOPERATIONS_H
