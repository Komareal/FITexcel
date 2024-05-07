//
// Created by ales on 5/4/24.
//

#ifndef TEST_H
#define TEST_H
#include "header.h"
#include "CSpreadsheet.h"
#include "Cell/CPos.h"
#include "Cell/expression.h"

void cellTest();
void runTests();
void posTest();
#ifdef BASIC_TEST
void basicTests();
#endif
#endif //TEST_H
