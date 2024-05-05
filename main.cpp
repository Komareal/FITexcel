#include "header.h"
#include "classes/CASTNode.h"
#include "classes/CCell.h"
#include "classes/expression.h"
#include "classes/CPos.h"
#include "classes/CSpreadsheet.h"
#include "classes/CBuilder.h"
#ifndef __PROGTEST__
#include "test.h"

int main() {
    // posTest();
    // cellTest();
    runTests();
#ifdef BASIC_TEST
    // basicTests();
#endif /* BASIC_TEST */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
