#include "csem_utils.h"

typedef struct Item {
    struct Item *parent;
    CSEM_List *types;
    CSEM_List *refs;
    char *id;
    CSEM_List *prop_name;
    CSEM_List *prop_value;
    CSEM_List *prop_type;
} Item;

typedef struct Id {
    char *value;
    CSEM_List *prop_name;
    CSEM_List *prop_value;
} Id;

typedef struct TestResult {
    CSEM_List *events;
    CSEM_List *items;
    CSEM_List *ids;
} TestResult;

Item *test_utils_createItem(CSEM_Bool createTypes, CSEM_Bool createRefs);
Id *test_utils_createId();
TestResult *test_utils_createTestResult();
void test_utils_disposeItem(Item *item, CSEM_Bool freeValue);
void test_utils_disposeId(Id *id, CSEM_Bool freeValue);
void test_utils_disposeItems(CSEM_List *items, CSEM_List *ids, CSEM_List *events, CSEM_Bool freeValue);
void test_utils_disposeTestActual(TestResult *actual, CSEM_Bool freeValue);
CU_ErrorCode test_utils_checkItem(Item *actual_item, Item *expected_item);
CU_ErrorCode test_utils_checkEvents(CSEM_List *actual, CSEM_List *expected);
CU_ErrorCode test_utils_checkItems(CSEM_List *actual, CSEM_List *expected);
CU_ErrorCode test_utils_checkIds(CSEM_List *actual, CSEM_List *expected);
