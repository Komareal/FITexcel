#ifndef __PROGTEST__
#include "test.h"
#endif /* __PROGTEST__ */

#ifndef __PROGTEST__
int main() {
    posTest();
    cellTest();

#ifdef BASIC_TEST
    basicTests();
#endif /* BASIC_TEST */

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
