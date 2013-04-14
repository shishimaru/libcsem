#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "csem_ns_manager.h"

void test_ns_manager() {
    CSEM_NSManager *manager = CSEM_NSManager_Create();

    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "a"), NULL);
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, NULL), NULL);
    CU_ASSERT_EQUAL(CSEM_NSManager_Push(manager), CSEM_ERROR_NONE);
    CSEM_NSManager_Pop(manager, CSEM_TRUE);

    //push
    CSEM_NSManager_Add(manager, "a", "http://a.org");
    CSEM_NSManager_Add(manager, "b", "http://b.org");
    CSEM_NSManager_Add(manager, "c", "http://c.org");
    CSEM_NSManager_Add(manager, NULL, "http://default.org");
    CSEM_NSManager_Push(manager);

    //test
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "a"), "http://a.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "b"), "http://b.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "c"), "http://c.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, NULL), "http://default.org");
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "d"), NULL);

    //push
    CSEM_NSManager_Add(manager, "a", "http://A.org");
    CSEM_NSManager_Add(manager, "b", "http://B.org");
    CSEM_NSManager_Add(manager, NULL, "http://DEFAULT.org");
    CSEM_NSManager_Push(manager);

    //test
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "a"), "http://A.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "b"), "http://B.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "c"), "http://c.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, NULL), "http://DEFAULT.org");
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "d"), NULL);

    //pop
    CSEM_NSManager_Pop(manager, CSEM_FALSE);

    //test
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "a"), "http://a.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "b"), "http://b.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, "c"), "http://c.org");
    CU_ASSERT_STRING_EQUAL(CSEM_NSManager_LookupURI(manager, NULL), "http://default.org");

    //pop
    CSEM_NSManager_Pop(manager, CSEM_FALSE);

    //test
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "a"), NULL);
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "b"), NULL);
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, "c"), NULL);
    CU_ASSERT_EQUAL(CSEM_NSManager_LookupURI(manager, NULL), NULL);

    CSEM_NSManager_Dispose(manager, CSEM_FALSE);
}
