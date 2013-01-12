#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "csem/csem_stream.h"

#include "csem_utils.h"
#include "test_micro_stream.h"

const int TYPE_STR = 0;
const int TYPE_URL = 1;
const int TYPE_ITEM = 2;

const int EVENT_START_SCOPE = 10;
const int EVENT_END_SCOPE = 11;
const int EVENT_START_PROP = 12;
const int EVENT_END_PROP = 13;
const int EVENT_PROP_VALUE = 14;
const int EVENT_START_ID = 15;
const int EVENT_END_ID = 16;

typedef struct Item Item;
struct Item {
    Item *parent;
    CSEM_List *types;
    CSEM_List *refs;
    char *id;
    CSEM_List *prop_name;
    CSEM_List *prop_value;
    CSEM_List *prop_type;
};

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

static Item *test_microdata_createItem(CSEM_Bool createTypes, CSEM_Bool createRefs) {
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
static Id *test_microdata_createId() {
    Id *id = CSEM_Calloc(1, sizeof(Id));
    id -> prop_name = CSEM_List_Create(8);
    id -> prop_value = CSEM_List_Create(8);
    return id;
}
static TestResult *test_microdata_createTestResult() {
    TestResult *result = CSEM_Calloc(1, sizeof(TestResult));
    result -> events = CSEM_List_Create(8);
    result -> items = CSEM_List_Create(8);
    result -> ids = CSEM_List_Create(8);
    return result;
}
static void test_microdata_disposeItem(Item *item, CSEM_Bool freeValue) {
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
            if(type == &TYPE_STR || type == &TYPE_URL) {
                if(freeValue) {
                    CSEM_Free(value);
                }
            } else {
                test_microdata_disposeItem(value, freeValue);
            }
        }
        CSEM_List_Dispose(item -> prop_value, CSEM_FALSE);
    }
    CSEM_List_Dispose(item -> prop_type, CSEM_FALSE);
    CSEM_Free(item);
}
static void test_microdata_disposeId(Id *id, CSEM_Bool freeValue) {
    if(freeValue) { CSEM_Free(id -> value); }
    CSEM_List_Dispose(id -> prop_name, freeValue);
    CSEM_List_Dispose(id -> prop_value, freeValue);
    CSEM_Free(id);
}
static void test_microdata_disposeItems(CSEM_List *items, CSEM_List *ids, CSEM_List *events, CSEM_Bool freeValue) {
    if(items) {
        int i = 0, size = 0;
        for(i = 0, size = CSEM_List_Size(items); i < size; i++) {
            Item *item = CSEM_List_Get(items, i);
            test_microdata_disposeItem(item, freeValue);
        }
        CSEM_List_Dispose(items, CSEM_FALSE);
    }
    if(ids) {
        int i = 0, size = 0;
        for(i = 0, size = CSEM_List_Size(ids); i < size; i++) {
            Id *id = CSEM_List_Get(ids, i);
            test_microdata_disposeId(id, freeValue);
        }
        CSEM_List_Dispose(ids, CSEM_FALSE);
    }
    if(events) {
        CSEM_List_Dispose(events, CSEM_FALSE);
    }
}
static void test_microdata_disposeTestActual(TestResult *actual, CSEM_Bool freeValue) {
    test_microdata_disposeItems(actual -> items, actual -> ids, actual -> events, freeValue);
    CSEM_Free(actual);
}
static CU_ErrorCode test_microdata_checkItem(Item *actual_item, Item *expected_item) {
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
            if(type == &TYPE_STR || type == &TYPE_URL) {
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(actual_item -> prop_value, i), CSEM_List_Get(expected_item -> prop_value, i));
            } else {
                test_microdata_checkItem(CSEM_List_Get(actual_item -> prop_value, i), CSEM_List_Get(expected_item -> prop_value, i));
            }
        }//*/
    }
    return CU_get_error();
}
static CU_ErrorCode test_microdata_checkEvents(CSEM_List *actual, CSEM_List *expected) {
    CU_ASSERT_EQUAL(CSEM_List_Size(actual), CSEM_List_Size(expected));
    int i = 0;
    int size = CSEM_List_Size(expected);
    for(i = 0; i < size; i++) {
        CU_ASSERT_PTR_EQUAL(CSEM_List_Get(actual, i), CSEM_List_Get(expected, i));
    }
    return CU_get_error();
}
static CU_ErrorCode test_microdata_checkItems(CSEM_List *actual, CSEM_List *expected) {
    CU_ASSERT_EQUAL(CSEM_List_Size(actual), CSEM_List_Size(expected));

    int itemSize = CSEM_List_Size(expected);
    int i = 0;
    for(i = 0; i < itemSize; i++) {
        Item *actual_item = CSEM_List_Get(actual, i);
        Item *expected_item = CSEM_List_Get(expected, i);
        test_microdata_checkItem(actual_item, expected_item);
    }
    return CU_get_error();
}
static CU_ErrorCode test_microdata_checkIds(CSEM_List *actual, CSEM_List *expected) {

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
Item *activeItem = NULL;
CSEM_Bool state_prop_value = CSEM_FALSE;
static CSEM_Bool handler_startScope(const void *userdata,
        const CSEM_List *types, const CSEM_List *refs,
        const char *id) {
    puts("@startScope");

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_SCOPE);

        Item *item = test_microdata_createItem(CSEM_FALSE, CSEM_FALSE);
        item -> types = (CSEM_List *)types;
        item -> refs = (CSEM_List *)refs;
        item -> id = (char *)id;
        item -> parent = activeItem;

        if(!item -> parent) {
            CSEM_List_Add(actual -> items, item);
        }
        if(state_prop_value) {
            CSEM_List_Add(activeItem -> prop_value, item);
            CSEM_List_Add(activeItem -> prop_type, &TYPE_ITEM);
        }
        activeItem = item;
    }

    int i = 0;
    {/* @itemtype */
        puts("@itemtype");
        size_t size = types ? CSEM_List_Size(types) : 0;
        for(i = 0; i < size; i++) {
            printf("%s\n", (char *)CSEM_List_Get(types, i));
        }
    }{/* @itemref */
        puts("@itemref");
        size_t size = refs ? CSEM_List_Size(refs) : 0;
        for(i = 0; i < size; i++) {
            printf("%s\n", (char *)CSEM_List_Get(refs, i));
        }
    }
    return CSEM_FALSE;
}
static void handler_endScope(const void *userdata) {
    puts("@endScope");
    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_END_SCOPE);
    }

    activeItem = activeItem -> parent;
}
static CSEM_Bool handler_startId(const void *userdata, const char *id) {
    puts("@startId");
    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_ID);

        Id *newid = test_microdata_createId();
        newid -> value = (char *)id;
        CSEM_List_Add(actual -> ids, newid);
    }
    return CSEM_FALSE;
}
static void handler_endId(const void *userdata) {
    puts("@endId");
    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_END_ID);
    }
}

static char *tmpPropValue = NULL;
static int *tmpValueType = NULL;
static CSEM_Bool handler_startProp(const void *userdata, const char *prop, CSEM_Bool hasUrlValue) {
    printf("@startProp=\"%s\"\n", prop);
    state_prop_value = CSEM_TRUE;
    tmpValueType = hasUrlValue ? &TYPE_URL : &TYPE_STR;

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_PROP);

        if(activeItem) {
            CSEM_List_Add(activeItem -> prop_name, prop);
        } else {
            int idSize = CSEM_List_Size(actual -> ids);
            Id *id = CSEM_List_Get(actual -> ids, idSize - 1);
            CSEM_List_Add(id -> prop_name, prop);
        }
    }
    tmpPropValue = NULL;
    return CSEM_FALSE;
}
static void handler_prop(const void *userdata, const char *value, int len) {
    fwrite(value, 1, len, stdout);

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_PROP_VALUE);
    }
    tmpPropValue = CSEM_Calloc(1024, sizeof(char));
    strncat(tmpPropValue, value, len);
}
static void handler_endProp(const void *userdata) {
    printf("@xendProp\n");
    state_prop_value = CSEM_FALSE;
    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_END_PROP);
    }

    if(tmpPropValue) {
        if(activeItem) {
            CSEM_List_Add(activeItem -> prop_value, tmpPropValue);
            CSEM_List_Add(activeItem -> prop_type, tmpValueType);
        } else {
            int idSize = CSEM_List_Size(actual -> ids);
            Id *id = CSEM_List_Get(actual -> ids, idSize - 1);
            CSEM_List_Add(id -> prop_value, tmpPropValue);
        }
    }
    tmpPropValue = NULL;
}
static void handler_error(const void *userdata, CSEM_Error error) {

}
void test_microdata_stream_basic() {
    TestResult *actual = test_microdata_createTestResult();
    TestResult *expected = test_microdata_createTestResult(); {/* setup expected results */
        {
            Item *item1 = test_microdata_createItem(CSEM_TRUE, CSEM_TRUE);
            CSEM_List_Add(item1 -> types, "http://sample.org/Person");
            CSEM_List_Add(item1 -> types, "http://sample2.org/Person");
            CSEM_List_Add(item1 -> refs, "address");
            CSEM_List_Add(item1 -> refs, "phone");
            item1 -> id = "urn:sample:0001";

            {/* prop name : values */
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "Neil");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "Nail");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "band");

                Item *item2 = test_microdata_createItem(CSEM_TRUE, CSEM_TRUE); {
                    CSEM_List_Add(item2 -> refs, "address");
                    CSEM_List_Add(item2 -> prop_name, "band-name");
                    CSEM_List_Add(item2 -> prop_value, "Four Parts Water");
                    CSEM_List_Add(item2 -> prop_type, &TYPE_STR);
                }
                CSEM_List_Add(item1 -> prop_value, item2);
                CSEM_List_Add(item1 -> prop_type, &TYPE_ITEM);
            }{
                CSEM_List_Add(item1 -> prop_name, "nationality home-country");
                CSEM_List_Add(item1 -> prop_value, "British");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }
            CSEM_List_Add(expected -> items, item1);
        }
        {
            Id *id = test_microdata_createId();
            id -> value = "address";
            {/* prop name : values*/
                CSEM_List_Add(id -> prop_name, "city");
                CSEM_List_Add(id -> prop_value, "Boston");
            }{
                CSEM_List_Add(id -> prop_name, "state");
                CSEM_List_Add(id -> prop_value, "MA");
            }
            CSEM_List_Add(expected -> ids, id);
        }{
            Id *id = test_microdata_createId();
            id -> value = "phone";
            {/* prop name : values */
                CSEM_List_Add(id -> prop_name, "num");
                CSEM_List_Add(id -> prop_value, "+1.617.981.xxxx");
            }{/* prop name : values */
                CSEM_List_Add(id -> prop_name, "name");
                CSEM_List_Add(id -> prop_value, "Android");
            }{/* prop name : values*/
                CSEM_List_Add(id -> prop_name, "dev");
                CSEM_List_Add(id -> prop_value, "VoIP");
            }
            CSEM_List_Add(expected -> ids, id);
        }{/* event sequence */
            CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP); {
                    CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
                        CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                        CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                        CSEM_List_Add(expected -> events, &EVENT_END_PROP);
                    }
                    CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);
                }
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);

            CSEM_List_Add(expected -> events, &EVENT_START_ID); {
                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_ID);

            CSEM_List_Add(expected -> events, &EVENT_START_ID); {
                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_ID);
        }
    }

    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Handler *handler;
    CSEM_Handler_Create(&handler);

    CSEM_Micro_Handlers *microdata;
    CSEM_Micro_CreateHandler(&microdata); {
        CSEM_Micro_SetStartScope(microdata, handler_startScope);
        CSEM_Micro_SetEndScope(microdata, handler_endScope);
        CSEM_Micro_SetStartId(microdata, handler_startId);
        CSEM_Micro_SetEndId(microdata, handler_endId);
        CSEM_Micro_SetStartItemProp(microdata, handler_startProp);
        CSEM_Micro_SetItemProp(microdata, handler_prop);
        CSEM_Micro_SetEndItemProp(microdata, handler_endProp);
    }
    CSEM_Handler_SetMicrodataHandler(handler, microdata);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);

    int fd = -1;

    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    if((error = CSEM_Parser_Parse(parser, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    /* check results */
    CU_ASSERT_EQUAL(test_microdata_checkEvents(actual -> events, expected -> events), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_microdata_checkItems(actual -> items, expected -> items), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_microdata_checkIds(actual -> ids, expected -> ids), CUE_SUCCESS);

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);

    test_microdata_disposeTestActual(expected, CSEM_FALSE);
    test_microdata_disposeTestActual(actual, CSEM_TRUE);
}
void test_microdata_stream_values() {
    TestResult *actual = test_microdata_createTestResult();
    TestResult *expected = test_microdata_createTestResult(); {/* setup expected results */
        {
            Item *item1 = test_microdata_createItem(CSEM_TRUE, CSEM_TRUE);
            CSEM_List_Add(item1 -> types, "http://sample.org/Person");
            CSEM_List_Add(item1 -> types, "http://sample2.org/Person");
            CSEM_List_Add(item1 -> refs, "address");
            CSEM_List_Add(item1 -> refs, "phone");
            item1 -> id = "urn:sample:0001";

            {/* prop name : values */
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "Neil");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "sample.mp3");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "sample.swf");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "frame");
                CSEM_List_Add(item1 -> prop_value, "sample.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "frame");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "sample.jpg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "sample.mp3");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "sample.m4v");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "sample.vtt");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "sample.mpeg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "licence");
                CSEM_List_Add(item1 -> prop_value, "licence.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "licence");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "top");
                CSEM_List_Add(item1 -> prop_value, "index.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "top");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "sample.jpg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URL);
            }{
                CSEM_List_Add(item1 -> prop_name, "animation");
                CSEM_List_Add(item1 -> prop_value, "sample.gif");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "animation");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "updated");
                CSEM_List_Add(item1 -> prop_value, "2000-01-01");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "updated");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }
            CSEM_List_Add(expected -> items, item1);
        }
        {/* ids */

        }
        {/* event sequence */
            CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);

                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);
        }
    }

    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Handler *handler;
    CSEM_Handler_Create(&handler);

    CSEM_Micro_Handlers *microdata;
    CSEM_Micro_CreateHandler(&microdata); {
        CSEM_Micro_SetStartScope(microdata, handler_startScope);
        CSEM_Micro_SetEndScope(microdata, handler_endScope);
        CSEM_Micro_SetStartId(microdata, handler_startId);
        CSEM_Micro_SetEndId(microdata, handler_endId);
        CSEM_Micro_SetStartItemProp(microdata, handler_startProp);
        CSEM_Micro_SetItemProp(microdata, handler_prop);
        CSEM_Micro_SetEndItemProp(microdata, handler_endProp);
    }
    CSEM_Handler_SetMicrodataHandler(handler, microdata);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);

    int fd = -1;

    if(!(fd = open("./data/5-4-Values.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    if((error = CSEM_Parser_Parse(parser, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    /* check results */
    CU_ASSERT_EQUAL(test_microdata_checkEvents(actual -> events, expected -> events), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_microdata_checkItems(actual -> items, expected -> items), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_microdata_checkIds(actual -> ids, expected -> ids), CUE_SUCCESS);

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);

    test_microdata_disposeTestActual(expected, CSEM_FALSE);
    test_microdata_disposeTestActual(actual, CSEM_TRUE);
}
