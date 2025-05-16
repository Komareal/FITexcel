#ifndef CASTOPERATIONS_H
#define CASTOPERATIONS_H
#include "../../header.h"


/**
 *  Class for static methods that perform operations on CSharedVal either binary, unary, or functions
 */
class CASTOperations {
public:
    /**
     *  adds two values
     *  either adds two doubles or concatenates two strings or string and double
     * @param first 
     * @param second 
     * @return result or nullptr if types are not compatible
     */
    static CSharedVal add(const CSharedVal &first, const CSharedVal &second);

    /**
     *  subtracts two doubles
     * @param first 
     * @param second 
     * @return result or nullptr if types are not compatible
     */
    static CSharedVal sub(const CSharedVal &first, const CSharedVal &second);

    /**
     *  multiplies two doubles
     * @param first 
     * @param second 
     * @return  result or nullptr if types are not compatible
     */
    static CSharedVal mul(const CSharedVal &first, const CSharedVal &second);

    /**
     *  divides two doubles
     * @param first 
     * @param second 
     * @return result or nullptr if arguments are not doubles
     */
    static CSharedVal div(const CSharedVal &first, const CSharedVal &second);

    /**
     *  raises first to the power of second
     * @param first 
     * @param second 
     * @return  result or nullptr if arguments are not doubles
     */
    static CSharedVal pow(const CSharedVal &first, const CSharedVal &second);

    /**
     * negates a double
     * @param val 
     * @return  result or nullptr if argument is not a double
     */
    static CSharedVal negate(const CSharedVal &val);

    /**
     *   returns 1.0 if both arguments are equal, 0.0 otherwise
     * @param first 
     * @param second 
     * @return  result or nullptr if types are not compatible
     */
    static CSharedVal eq(const CSharedVal &first, const CSharedVal &second);

    /**
     * returns 1.0 if both arguments are not equal, 0.0 otherwise
     * @param first
     * @param second 
     * @return result or nullptr if types are not compatible
     */
    static CSharedVal ne(const CSharedVal &first, const CSharedVal &second);

    /**
     *  returns 1.0 if first is less than second, 0.0 otherwise
     * @param first
     * @param second
     * @return  result or nullptr if types are not compatible
     */
    static CSharedVal lt(const CSharedVal &first, const CSharedVal &second);

    /**
     *  returns 1.0 if first is less than or equal to second, 0.0 otherwise
     * @param first
     * @param second
     * @return  result or nullptr if types are not compatible
     */
    static CSharedVal le(const CSharedVal &first, const CSharedVal &second);

    /**
     *  returns 1.0 if first is greater than second, 0.0 otherwise
     * @param first
     * @param second
     * @return result or nullptr if types are not compatible
     */
    static CSharedVal gt(const CSharedVal &first, const CSharedVal &second);

    /**
     *  returns 1.0 if first is greater than or equal to second, 0.0 otherwise
     * @param first
     * @param second
     * @return  result or nullptr if types are not compatible
     */
    static CSharedVal ge(const CSharedVal &first, const CSharedVal &second);

    /**
     *  is used to perform sum function in range loop
     * @param carry carry of previous results
     * @param val value to add to carry
     */
    static void sum(CSharedVal &carry, const CSharedVal &val);

    /**
     *  is used to count defined values in range loop
     * @param carry  carry of previous results
     * @param val  if value is not nullptr, carry is incremented
     */
    static void cnt(const CSharedVal &carry, const CSharedVal &val);

    /**
     *  is used to find minimum value in range loop 
     * @param carry previous minimum
     * @param val value to compare with carry
     */
    static void min(CSharedVal &carry, const CSharedVal &val);

    /**
     *  is used to find maximum value in range loop
     * @param carry previous maximum
     * @param val value to compare with carry
     */
    static void max(CSharedVal &carry, const CSharedVal &val);

    /**
     *  is used to count values in range loop that are equal to lookFor
     * @param carry  carry of previous results, if val and lookFor are equal, carry is incremented
     * @param val  value to compare with lookFor
     * @param lookFor  value to compare with val
     */
    static void cntVal(const CSharedVal &carry, const CSharedVal &val, const CSharedVal &lookFor);

private:
    /**
     *  checks if two values are doubles
     * @param first
     * @param second
     * @return  true if both are doubles, false otherwise
     */
    static bool assumeDoubles(const CSharedVal &first, const CSharedVal &second);

    /**
     *  checks if two values are strings
     * @param first
     * @param second
     * @return  true if both are strings, false otherwise
     */
    static bool assumeStrings(const CSharedVal &first, const CSharedVal &second);

    /**
     *  checks if first is string and second is double
     * @param first
     * @param second
     * @return  true if first is string and second is double, false otherwise
     */
    static bool assumeSD(const CSharedVal &first, const CSharedVal &second);

    /**
     *  checks if first is double and second is string
     * @param first
     * @param second
     * @return  true if first is double and second is string, false otherwise
     */
    static bool assumeDS(const CSharedVal &first, const CSharedVal &second);

    /**
     *  converts bool to double
     * @param x
     * @return  1.0 if x is true, 0.0 otherwise
     */
    static double convertBoolToDouble(bool x);

    /**
     *  If double is 0.0, returns 1.0, otherwise 0.0
     * @param val
     * @return
     */
    static CSharedVal negateBool(const CSharedVal &val);
};


#endif //CASTOPERATIONS_H
