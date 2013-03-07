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

static Item *activeItem = NULL;
static CSEM_Bool state_prop_value = CSEM_FALSE;
static CSEM_Bool handler_startScope(const void *userdata,
        const CSEM_Url *resource, const CSEM_List *types){
    puts("@startScope");

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_SCOPE);

        Item *item = test_utils_createItem(CSEM_FALSE, CSEM_FALSE);
        item -> types = (CSEM_List *)types;
        {/* resource */
            CSEM_URL_Serialize(resource, &(item -> id));
            CSEM_URL_Dispose((CSEM_Url *)resource);
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

static char *tmpPropValue = NULL;
static int tmpPropValueReadLen = 0;
static int tmpPropValueLen = 0;
static int *tmpValueType = NULL;
static CSEM_Bool handler_startProp(const void *userdata,
        const char *propName, CSEM_Bool hasUrlValue) {
    printf("@startProp=\"%s\"\n", propName);
    state_prop_value = CSEM_TRUE;
    tmpValueType = hasUrlValue ? &TYPE_URI : &TYPE_STR;

    TestResult *actual = (void *)userdata; {
        CSEM_List_Add(actual -> events, &EVENT_START_PROP);
        CSEM_List_Add(activeItem -> prop_name, propName);
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
void test_rdfa_stream_basic() {
    TestResult *actual = test_utils_createTestResult();
    TestResult *expected = test_utils_createTestResult(); {/* setup expected results */
        {
            Item *item1 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            item1 -> id = "http://localhost/test/main";
            CSEM_List_Add(item1 -> types, "http://schema.org/Person");
            CSEM_List_Add(item1 -> types, "http://schema.org/Person");

            {/* prop name : values */
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "Neil");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "name");
                CSEM_List_Add(item1 -> prop_value, "Nail");
                CSEM_List_Add(item1 -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item1 -> prop_name, "jobTitle");

                Item *item2 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE); {
                    item2 -> id = "http://localhost/test/sub1";
                    CSEM_List_Add(item2 -> types, "http://schema2.org/MusicRecording");
                    CSEM_List_Add(item2 -> types, "http://schema2.org/MusicRecording");
                    CSEM_List_Add(item2 -> prop_name, "byArtist");
                    CSEM_List_Add(item2 -> prop_value, "Four \"Parts\" Water");
                    CSEM_List_Add(item2 -> prop_type, &TYPE_STR);
                }
                CSEM_List_Add(item1 -> prop_value, item2);
                CSEM_List_Add(item1 -> prop_type, &TYPE_ITEM);
            }{
                CSEM_List_Add(item1 -> prop_name, "jobTitle");

                Item *item2 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE); {
                    item2 -> id = "http://localhost/test/sub2";
                    CSEM_List_Add(item2 -> types, "http://schema.org/MusicRecording");
                    CSEM_List_Add(item2 -> types, "http://schema2.org/MusicRecording");
                    CSEM_List_Add(item2 -> prop_name, "byArtist");
                    CSEM_List_Add(item2 -> prop_value, "Four \"Parts\" Water");
                    CSEM_List_Add(item2 -> prop_type, &TYPE_STR);
                }
                CSEM_List_Add(item1 -> prop_value, item2);
                CSEM_List_Add(item1 -> prop_type, &TYPE_ITEM);
            }{
                CSEM_List_Add(item1 -> prop_name, "address");

                Item *item2 = test_utils_createItem(CSEM_TRUE, CSEM_TRUE); {
                    CSEM_List_Add(item2 -> types, "http://schema.org/Person");
                    CSEM_List_Add(item2 -> types, "http://schema.org/Person");
                    CSEM_List_Add(item2 -> prop_name, "homeLocation");
                    CSEM_List_Add(item2 -> prop_value, "British");
                    CSEM_List_Add(item2 -> prop_type, &TYPE_STR);
                }
                CSEM_List_Add(item1 -> prop_value, item2);
                CSEM_List_Add(item1 -> prop_type, &TYPE_ITEM);
            }
            CSEM_List_Add(expected -> items, item1);

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

                CSEM_List_Add(expected -> events, &EVENT_START_PROP); {
                    CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
                        CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                        CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                        CSEM_List_Add(expected -> events, &EVENT_END_PROP);
                    }
                    CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);
                }
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);
        }
    }

    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Handler *handler;
    CSEM_Handler_Create(&handler);

    CSEM_RDFa_Handlers *rdfa;
    CSEM_RDFa_CreateHandler(&rdfa); {
        CSEM_RDFa_SetItemStart(rdfa, handler_startScope);
        CSEM_RDFa_SetItemEnd(rdfa, handler_endScope);
        CSEM_RDFa_SetPropStart(rdfa, handler_startProp);
        CSEM_RDFa_SetPropValue(rdfa, handler_prop);
        CSEM_RDFa_SetPropEnd(rdfa, handler_endProp);
    }
    CSEM_Handler_SetRDFaHandler(handler, rdfa);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);
    CSEM_Url *baseURL; {
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Parser_SetBaseURL(parser, baseURL);
    }

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-basic.html", O_RDONLY))) {
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

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);
    CSEM_URL_Dispose(baseURL);

    test_utils_disposeTestActual(expected, CSEM_FALSE);
    test_utils_disposeTestActual(actual, CSEM_TRUE);
}
void test_rdfa_stream_vocab() {
    TestResult *actual = test_utils_createTestResult();
    TestResult *expected = test_utils_createTestResult(); {/* setup expected results */
        {
            Item *item = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            item -> id = "http://localhost/test/#manu";
            CSEM_List_Add(item -> types, "http://xmlns.com/foaf/0.1/Person");

            {
                CSEM_List_Add(item -> prop_name, "name");
                CSEM_List_Add(item -> prop_value, "Manu Sporny");
                CSEM_List_Add(item -> prop_type, &TYPE_STR);
            }{
                CSEM_List_Add(item -> prop_name, "knows");
                CSEM_List_Add(item -> prop_value, "#alex");
                CSEM_List_Add(item -> prop_type, &TYPE_URI);
            }{
                CSEM_List_Add(item -> prop_name, "knows");
                CSEM_List_Add(item -> prop_value, "#brian");
                CSEM_List_Add(item -> prop_type, &TYPE_URI);
            }
            CSEM_List_Add(expected -> items, item);
        }{
            Item *item = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            item -> id = "http://localhost/test/#alex";
            CSEM_List_Add(item -> types, "http://xmlns.com/foaf/0.1/Person");

            {
                CSEM_List_Add(item -> prop_name, "name");
                CSEM_List_Add(item -> prop_value, "Alex Milowski");
                CSEM_List_Add(item -> prop_type, &TYPE_STR);
            }
            CSEM_List_Add(expected -> items, item);
        }{
            Item *item = test_utils_createItem(CSEM_TRUE, CSEM_TRUE);
            item -> id = "http://localhost/test/#brian";
            CSEM_List_Add(item -> types, "http://xmlns.com/foaf/0.1/Person");

            {
                CSEM_List_Add(item -> prop_name, "name");
                CSEM_List_Add(item -> prop_value, "Brian Sletten");
                CSEM_List_Add(item -> prop_type, &TYPE_STR);
            }
            CSEM_List_Add(expected -> items, item);
        }{/* event sequence */
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
            }
            CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);

            CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
                CSEM_List_Add(expected -> events, &EVENT_START_PROP);
                CSEM_List_Add(expected -> events, &EVENT_PROP_VALUE);
                CSEM_List_Add(expected -> events, &EVENT_END_PROP);
            }
            CSEM_List_Add(expected -> events, &EVENT_END_SCOPE);

            CSEM_List_Add(expected -> events, &EVENT_START_SCOPE); {
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

    CSEM_RDFa_Handlers *rdfa;
    CSEM_RDFa_CreateHandler(&rdfa); {
        CSEM_RDFa_SetItemStart(rdfa, handler_startScope);
        CSEM_RDFa_SetItemEnd(rdfa, handler_endScope);
        CSEM_RDFa_SetPropStart(rdfa, handler_startProp);
        CSEM_RDFa_SetPropValue(rdfa, handler_prop);
        CSEM_RDFa_SetPropEnd(rdfa, handler_endProp);
    }
    CSEM_Handler_SetRDFaHandler(handler, rdfa);
    CSEM_Handler_SetErrorHandler(handler, handler_error);

    CSEM_Parser *parser;
    if((error = CSEM_Parser_Create(&parser))) {
        goto FINISH;
    }
    CSEM_Parser_SetUserdata(parser, actual);
    CSEM_Parser_SetHandler(parser, handler);
    CSEM_Url *baseURL;
    CSEM_URL_Parse("http://localhost/test/", &baseURL);
    CSEM_Parser_SetBaseURL(parser, baseURL);

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-ssn.html", O_RDONLY))) {
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

FINISH:
    CSEM_Parser_Dispose(parser);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);
    CSEM_URL_Dispose(baseURL);

    test_utils_disposeTestActual(expected, CSEM_FALSE);
    test_utils_disposeTestActual(actual, CSEM_TRUE);
}
