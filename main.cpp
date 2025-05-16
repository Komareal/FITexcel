#include "src/header.h"

#ifndef __PROGTEST__
#include "test/test.h"

int main() {
    posTest();
    cellTest();
    basicTests();
    funcTest();
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
