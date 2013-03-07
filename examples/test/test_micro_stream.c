#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "csem/csem_stream.h"
#include "test_utils_stream.h"

extern int TYPE_STR;
extern int TYPE_URI;
extern int TYPE_ITEM;

extern const int EVENT_START_SCOPE;
extern const int EVENT_END_SCOPE;
extern const int EVENT_START_PROP;
extern const int EVENT_END_PROP;
extern const int EVENT_PROP_VALUE;
extern const int EVENT_START_ID;
extern const int EVENT_END_ID;

static Item *activeItem = NULL;
static CSEM_Bool state_prop_value = CSEM_FALSE;
static CSEM_Bool handler_startScope(const void *userdata,
        const CSEM_Url *id, const CSEM_List *types, const CSEM_List *refs) {
    puts("@startScope");

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_SCOPE);

        Item *item = test_utils_createItem(CSEM_FALSE, CSEM_FALSE);
        item -> types = (CSEM_List *)types;
        item -> refs = (CSEM_List *)refs;
        {
            CSEM_URL_Serialize(id, &(item -> id));
            CSEM_URL_Dispose((CSEM_Url *)id);
        }
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

        Id *newid = test_utils_createId();
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
static int tmpPropValueReadLen = 0;
static int tmpPropValueLen = 0;
static int *tmpValueType = NULL;
static CSEM_Bool handler_startProp(const void *userdata, const char *prop, CSEM_Bool hasUrlValue) {
    printf("@startProp=\"%s\"\n", prop);
    state_prop_value = CSEM_TRUE;
    tmpValueType = hasUrlValue ? &TYPE_URI : &TYPE_STR;

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
    printf("@propValue=");
    fwrite(value, 1, len, stdout);
    printf("\n");

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_PROP_VALUE);
    }
    if(!tmpPropValue) {
        tmpPropValue = CSEM_Calloc(len + 1, sizeof(char));
        tmpPropValueReadLen = 0;
        tmpPropValueLen = len + 1;
    } else {
        if(tmpPropValueReadLen + len > tmpPropValueLen - 1) {
            tmpPropValue = CSEM_Realloc(tmpPropValue, tmpPropValueLen + len + 1);
            tmpPropValueLen += len + 1;
        }
    }
    strncat(tmpPropValue + tmpPropValueReadLen, value, len);
    tmpPropValueReadLen += len;
}
static void handler_endProp(const void *userdata) {
    printf("@endProp\n");
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
    TestResult *actual = test_utils_createTestResult();
    TestResult *expected = test_utils_createTestResult(); {/* setup expected results */
        {
            Item *item1 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            CSEM_List_Add(item1 -> types, "http://sample.org/Person");
            CSEM_List_Add(item1 -> types, "http://sample2.org/Person");
            CSEM_List_Add(item1 -> refs, "address");
            CSEM_List_Add(item1 -> refs, "phone");
            item1 -> id = "http://localhost/test/neil3655";

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

                Item *item2 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE); {
                    CSEM_List_Add(item2 -> refs, "address");
                    CSEM_List_Add(item2 -> prop_name, "band-name");
                    CSEM_List_Add(item2 -> prop_value, "Four \"Parts\" Water");
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
            Id *id = test_utils_createId();
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
            Id *id = test_utils_createId();
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
                        CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                        CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                        CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
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
        CSEM_Micro_SetItemStart(microdata, handler_startScope);
        CSEM_Micro_SetItemEnd(microdata, handler_endScope);
        CSEM_Micro_SetIdStart(microdata, handler_startId);
        CSEM_Micro_SetIdEnd(microdata, handler_endId);
        CSEM_Micro_SetPropStart(microdata, handler_startProp);
        CSEM_Micro_SetPropValue(microdata, handler_prop);
        CSEM_Micro_SetPropEnd(microdata, handler_endProp);
    }
    CSEM_Handler_SetMicrodataHandler(handler, microdata);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Parser_SetBaseURL(parser, baseURL);
    }

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    if((error = CSEM_Parser_Parse(parser, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    /* check results */
    CU_ASSERT_EQUAL(test_utils_checkEvents(actual -> events, expected -> events), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_utils_checkItems(actual -> items, expected -> items), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_utils_checkIds(actual -> ids, expected -> ids), CUE_SUCCESS);

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);
    CSEM_URL_Dispose(baseURL);

    test_utils_disposeTestActual(expected, CSEM_FALSE);
    test_utils_disposeTestActual(actual, CSEM_TRUE);
}
void test_microdata_stream_values() {
    TestResult *actual = test_utils_createTestResult();
    TestResult *expected = test_utils_createTestResult(); {/* setup expected results */
        {
            Item *item1 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            CSEM_List_Add(item1 -> types, "http://sample.org/Person");
            CSEM_List_Add(item1 -> types, "http://sample2.org/Person");
            CSEM_List_Add(item1 -> refs, "address");
            CSEM_List_Add(item1 -> refs, "phone");
            item1 -> id = "http://sample.org:8080/";

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
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "sample.swf");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "frame");
                CSEM_List_Add(item1 -> prop_value, "sample.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "frame");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "sample.jpg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "sample.mp3");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "music");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "sample.m4v");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "sample.vtt");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "video");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "sample.mpeg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "movie");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "licence");
                CSEM_List_Add(item1 -> prop_value, "licence.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "licence");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "top");
                CSEM_List_Add(item1 -> prop_value, "index.html");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "top");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "sample.jpg");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item1 -> prop_name, "photo");
                CSEM_List_Add(item1 -> prop_value, "");
                CSEM_List_Add(item1 -> prop_type, &TYPE_URI);
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
        CSEM_Micro_SetItemStart(microdata, handler_startScope);
        CSEM_Micro_SetItemEnd(microdata, handler_endScope);
        CSEM_Micro_SetIdStart(microdata, handler_startId);
        CSEM_Micro_SetIdEnd(microdata, handler_endId);
        CSEM_Micro_SetPropStart(microdata, handler_startProp);
        CSEM_Micro_SetPropValue(microdata, handler_prop);
        CSEM_Micro_SetPropEnd(microdata, handler_endProp);
    }
    CSEM_Handler_SetMicrodataHandler(handler, microdata);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Parser_SetBaseURL(parser, baseURL);
    }

    int fd = -1;
    if(!(fd = open("./data/micro-5-4-Values.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    if((error = CSEM_Parser_Parse(parser, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    /* check results */
    CU_ASSERT_EQUAL(test_utils_checkEvents(actual -> events, expected -> events), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_utils_checkItems(actual -> items, expected -> items), CUE_SUCCESS);
    CU_ASSERT_EQUAL(test_utils_checkIds(actual -> ids, expected -> ids), CUE_SUCCESS);

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);
    CSEM_URL_Dispose(baseURL);

    test_utils_disposeTestActual(expected, CSEM_FALSE);
    test_utils_disposeTestActual(actual, CSEM_TRUE);
}
