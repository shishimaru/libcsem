#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "test_utils_stream.h"

const int TYPE_STR = 0;
const int TYPE_URI = 1;
const int TYPE_ITEM = 2;

const int EVENT_START_SCOPE = 10;
const int EVENT_END_SCOPE = 11;
const int EVENT_START_PROP = 12;
const int EVENT_END_PROP = 13;
const int EVENT_PROP_VALUE = 14;
const int EVENT_START_ID = 15;
const int EVENT_END_ID = 16;

Item *test_utils_createItem(CSEM_Bool createTypes, CSEM_Bool createRefs) {
    Item *item = CSEM_Calloc(1, sizeof(Item));
    if(createTypes) {
        item -> types = CSEM_List_Create(8);
    }
    if(createRefs) {
        item -> refs = CSEM_List_Create(8);
    }
    item -> prop_name = CSEM_List_Create(8);
    item -> prop_value = CSEM_List_Create(8);
    item -> prop_type = CSEM_List_Create(8);
    return item;
}
Id *test_utils_createId() {
    Id *id = CSEM_Calloc(1, sizeof(Id));
    id -> prop_name = CSEM_List_Create(8);
    id -> prop_value = CSEM_List_Create(8);
    return id;
}
TestResult *test_utils_createTestResult() {
    TestResult *result = CSEM_Calloc(1, sizeof(TestResult));
    result -> events = CSEM_List_Create(8);
    result -> items = CSEM_List_Create(8);
    result -> ids = CSEM_List_Create(8);
    return result;
}
void test_utils_disposeItem(Item *item, CSEM_Bool freeValue) {
    if(item -> types) {
        CSEM_List_Dispose(item -> types, freeValue);
    }
    if(item -> refs) {
        CSEM_List_Dispose(item -> refs, freeValue);
    }
    if(item -> id && freeValue) { CSEM_Free(item -> id); }
    CSEM_List_Dispose(item -> prop_name, freeValue);

    {/* free values */
        int i = 0;
        int size = CSEM_List_Size(item -> prop_value);
        for(i = 0; i < size; i++) {
            void *value = CSEM_List_Get(item -> prop_value, i);
            int *type = CSEM_List_Get(item -> prop_type, i);
            if(type == &TYPE_STR || type == &TYPE_URI) {
                if(freeValue) {
                    CSEM_Free(value);
                }
            } else {
                test_utils_disposeItem(value, freeValue);
            }
        }
        CSEM_List_Dispose(item -> prop_value, CSEM_FALSE);
    }
    CSEM_List_Dispose(item -> prop_type, CSEM_FALSE);
    CSEM_Free(item);
}
void test_utils_disposeId(Id *id, CSEM_Bool freeValue) {
    if(freeValue) { CSEM_Free(id -> value); }
    CSEM_List_Dispose(id -> prop_name, freeValue);
    CSEM_List_Dispose(id -> prop_value, freeValue);
    CSEM_Free(id);
}
void test_utils_disposeItems(CSEM_List *items, CSEM_List *ids, CSEM_List *events, CSEM_Bool freeValue) {
    if(items) {
        int i = 0, size = 0;
        for(i = 0, size = CSEM_List_Size(items); i < size; i++) {
            Item *item = CSEM_List_Get(items, i);
            test_utils_disposeItem(item, freeValue);
        }
        CSEM_List_Dispose(items, CSEM_FALSE);
    }
    if(ids) {
        int i = 0, size = 0;
        for(i = 0, size = CSEM_List_Size(ids); i < size; i++) {
            Id *id = CSEM_List_Get(ids, i);
            test_utils_disposeId(id, freeValue);
        }
        CSEM_List_Dispose(ids, CSEM_FALSE);
    }
    if(events) {
        CSEM_List_Dispose(events, CSEM_FALSE);
    }
}
void test_utils_disposeTestActual(TestResult *actual, CSEM_Bool freeValue) {
    test_utils_disposeItems(actual -> items, actual -> ids, actual -> events, freeValue);
    CSEM_Free(actual);
}
CU_ErrorCode test_utils_checkItem(Item *actual_item, Item *expected_item) {
    int i = 0;
    if(actual_item -> types) {/* types */
        CU_ASSERT_EQUAL(CSEM_List_Size(actual_item -> types), CSEM_List_Size(expected_item -> types));
        int size = CSEM_List_Size(expected_item -> types);
        for(i = 0; i < size; i++) {
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_item -> types, i), CSEM_List_Get(expected_item -> types, i));
        }
    }
    if(actual_item -> refs){/* refs */
        CU_ASSERT_EQUAL(CSEM_List_Size(actual_item -> refs), CSEM_List_Size(expected_item -> refs));
        int size = CSEM_List_Size(expected_item -> refs);
        for(i = 0; i < size; i++) {
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_item -> refs, i), CSEM_List_Get(expected_item -> refs, i));
        }
    }
    if(actual_item -> id) {/* id */
        CU_ASSERT_STRING_EQUAL(actual_item -> id, expected_item -> id);
    }
    {/* prop name : value */
        CU_ASSERT_EQUAL(CSEM_List_Size(actual_item -> prop_name), CSEM_List_Size(expected_item -> prop_name));
        CU_ASSERT_EQUAL(CSEM_List_Size(actual_item -> prop_value), CSEM_List_Size(expected_item -> prop_value));

        int size = CSEM_List_Size(expected_item -> prop_name);
        for(i = 0; i < size; i++) {
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_item -> prop_name, i), CSEM_List_Get(expected_item -> prop_name, i));
            CU_ASSERT_PTR_EQUAL(CSEM_List_Get(actual_item -> prop_type, i), CSEM_List_Get(expected_item -> prop_type, i));

            int *type = CSEM_List_Get(expected_item -> prop_type, i);
            if(type == &TYPE_STR || type == &TYPE_URI) {
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_item -> prop_value, i), CSEM_List_Get(expected_item -> prop_value, i));
            } else {
                test_utils_checkItem(CSEM_List_Get(actual_item -> prop_value, i), CSEM_List_Get(expected_item -> prop_value, i));
            }
        }//*/
    }
    return CU_get_error();
}
CU_ErrorCode test_utils_checkEvents(CSEM_List *actual, CSEM_List *expected) {
    CU_ASSERT_EQUAL(CSEM_List_Size(actual), CSEM_List_Size(expected));
    int i = 0;
    int size = CSEM_List_Size(expected);
    for(i = 0; i < size; i++) {
        CU_ASSERT_PTR_EQUAL(CSEM_List_Get(actual, i), CSEM_List_Get(expected, i));
    }
    return CU_get_error();
}
CU_ErrorCode test_utils_checkItems(CSEM_List *actual, CSEM_List *expected) {
    CU_ASSERT_EQUAL(CSEM_List_Size(actual), CSEM_List_Size(expected));

    int itemSize = CSEM_List_Size(expected);
    int i = 0;
    for(i = 0; i < itemSize; i++) {
        Item *actual_item = CSEM_List_Get(actual, i);
        Item *expected_item = CSEM_List_Get(expected, i);
        test_utils_checkItem(actual_item, expected_item);
    }
    return CU_get_error();
}
CU_ErrorCode test_utils_checkIds(CSEM_List *actual, CSEM_List *expected) {
    if(!expected) {
        CU_ASSERT_EQUAL(actual, expected);
    } else {
        CU_ASSERT_EQUAL(CSEM_List_Size(actual), CSEM_List_Size(expected));

        int size = CSEM_List_Size(expected);
        int i = 0;
        for(i = 0; i < size; i++) {
            Id *actual_id = CSEM_List_Get(actual, i);
            Id *expected_id = CSEM_List_Get(expected, i);
            CU_ASSERT_STRING_EQUAL(actual_id -> value, expected_id -> value);

            int j = 0;
            {/* prop name : value */
                CU_ASSERT_EQUAL(CSEM_List_Size(actual_id -> prop_name), CSEM_List_Size(expected_id -> prop_name));
                CU_ASSERT_EQUAL(CSEM_List_Size(actual_id -> prop_value), CSEM_List_Size(expected_id -> prop_value));

                int propSize = CSEM_List_Size(expected_id -> prop_name);
                for(j = 0; j < propSize; j++) {
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_id -> prop_name, j), CSEM_List_Get(expected_id -> prop_name, j));
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_id -> prop_value, j), CSEM_List_Get(expected_id -> prop_value, j));
                }
            }
        }
    }
    return CU_get_error();
}
