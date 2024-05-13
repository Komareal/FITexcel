#ifndef __PROGTEST__
#ifndef TEST_H
#define TEST_H
#include "header.h"
#include "CSpreadsheet.h"
#include "Cell/CPos.h"
#include "Cell/expression.h"

/**
 * Tests the functionality of CPos
 */
void posTest();

/**
 *  Tests basic functionality of cells/spreadsheet
 */
void cellTest();

/**
  *  Tests the functionality of the functions in the cells
 */
void funcTest();

/**
 *  Default tests provided by the progtest
 */
void basicTests();
#endif //TEST_H
#endif //__PROGTEST__
