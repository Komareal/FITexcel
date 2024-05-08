//
// Created by ales on 5/6/24.
//

#include "CASTOperations.h"

#include <memory>
#include <memory>

CSharedVal CASTOperations::add(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(std::get<double>(*first) + std::get<double>(*second));

    if (assumeStrings(first, second))
        return std::make_shared<CValue>(std::get<std::string>(*first) + std::get<std::string>(*second));

    if (assumeDS(first, second))
        return std::make_shared<CValue>(
            std::to_string(std::get<double>(*first))
            + std::get<std::string>(*second)
        );

    if (assumeSD(first, second)) {
        return std::make_shared<CValue>(
            std::get<std::string>(*first) +
            std::to_string(std::get<double>(*second))
        );
    }
    return nullptr;
}

CSharedVal CASTOperations::sub(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(std::get<double>(*first) - std::get<double>(*second));
    return nullptr;
}

CSharedVal CASTOperations::mul(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(std::get<double>(*first) * std::get<double>(*second));
    return nullptr;
}

CSharedVal CASTOperations::div(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second)) {
        const double secondVal = std::get<double>(*second);

        if (secondVal == 0)
            return nullptr;

        return std::make_shared<CValue>(std::get<double>(*first) / secondVal);
    }
    return nullptr;
}

CSharedVal CASTOperations::pow(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(
            std::pow(
                std::get<double>(*first),
                std::get<double>(*second)
            )
        );
    return nullptr;
}

CSharedVal CASTOperations::negate(const CSharedVal &val) {
    if (val != nullptr && holds_alternative<double>(*val))
        return std::make_shared<CValue>(
            -std::get<double>(*val)
        );
    return nullptr;
}

CSharedVal CASTOperations::eq(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(
            convertBoolToDouble(
                std::get<double>(*first) == std::get<double>(*second)
            )
        );
    if (assumeStrings(first, second))
        return std::make_shared<CValue>(
            convertBoolToDouble(
                std::get<std::string>(*first) == std::get<std::string>(*second)
            )
        );
    return nullptr;
}

CSharedVal CASTOperations::ne(const CSharedVal &first, const CSharedVal &second) {
    return negateBool(eq(first, second));
}

CSharedVal CASTOperations::lt(const CSharedVal &first, const CSharedVal &second) {
    if (assumeDoubles(first, second))
        return std::make_shared<CValue>(
            convertBoolToDouble(
                std::get<double>(*first) < std::get<double>(*second)
            )
        );
    if (assumeStrings(first, second))
        return std::make_shared<CValue>(
            convertBoolToDouble(
                std::get<std::string>(*first) < std::get<std::string>(*second)
            )
        );
    return nullptr;
}

CSharedVal CASTOperations::gt(const CSharedVal &first, const CSharedVal &second) {
    return lt(second, first);
}

CSharedVal CASTOperations::ge(const CSharedVal &first, const CSharedVal &second) {
    return negateBool(lt(first, second));
}

CSharedVal CASTOperations::le(const CSharedVal &first, const CSharedVal &second) {
    return ge(second, first);
}


bool CASTOperations::assumeDoubles(const CSharedVal &first, const CSharedVal &second) {
    return holds_alternative<double>(*first) && holds_alternative<double>(*second);
}

bool CASTOperations::assumeStrings(const CSharedVal &first, const CSharedVal &second) {
    return holds_alternative<std::string>(*first) && holds_alternative<std::string>(*second);
}

bool CASTOperations::assumeSD(const CSharedVal &first, const CSharedVal &second) {
    return holds_alternative<std::string>(*first) && holds_alternative<double>(*second);
}

bool CASTOperations::assumeDS(const CSharedVal &first, const CSharedVal &second) {
    return holds_alternative<double>(*first) && holds_alternative<std::string>(*second);
}

double CASTOperations::convertBoolToDouble(const bool x) {
    return x ? 1.0 : 0.0;
}

CSharedVal CASTOperations::negateBool(const CSharedVal &val) {
    if (val == nullptr)
        return nullptr;
    const CSharedVal res = std::make_shared<CValue>(*val);
    *res = convertBoolToDouble(std::get<double>(*res) == 0);
    return res;
}
