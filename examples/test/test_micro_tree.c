#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <CUnit/CUnit.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

#include "csem_utils.h"
#include "test_micro_tree.h"

void test_microdata_tree_values() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-5-4-Values.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://sample.org:8080/");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 28);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* name : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* music : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mp3");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* movie : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.swf");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* frame : sample.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "frame");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* frame : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "frame");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* photo : sample.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.jpg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* photo : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 9);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* music : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 10);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mp3");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 11);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* video : sample.m4v */
                    CSEM_Property *property = CSEM_List_Get(properties, 12);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.m4v");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* video : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 13);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* video : sample.vtt */
                    CSEM_Property *property = CSEM_List_Get(properties, 14);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.vtt");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* video : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 15);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* movie : sample.mpeg */
                    CSEM_Property *property = CSEM_List_Get(properties, 16);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mpeg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* movie : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 17);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* licence : sample.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 18);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "licence");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "licence.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* licence : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 19);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "licence");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* top : index.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 20);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "top");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "index.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* top : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 21);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "top");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* photo : sample.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 22);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.jpg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* photo : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 23);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                }
                {/* animation : sample.gif */
                    CSEM_Property *property = CSEM_List_Get(properties, 24);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "animation");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.gif");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* animation : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 25);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "animation");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* updated : 2000-01-01 */
                    CSEM_Property *property = CSEM_List_Get(properties, 26);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "updated");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "2000-01-01");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* updated : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 27);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "updated");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
            }
        }
    }

FINISH:
    CSEM_URL_Dispose(baseURL);
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_no_microdata() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/no-microdata.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 0);
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_recursive_itemprop() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-recursive-itemprop.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/root");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(types, NULL);
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 3);
                {/* @itemprop : A */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "A");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* str("a") */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "a");
                    }
                    {/* @itemprop : B */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);
                        CSEM_Property *property_b = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Property_GetNames(property_b);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "B");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Property_GetValues(property_b, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                        {/* str("b") */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "b");
                        }
                        {/* @itemprop : C */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);
                            CSEM_Property *property_c = CSEM_List_Get(values, 1);
                            CSEM_List *names = CSEM_Property_GetNames(property_c);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "C");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Property_GetValues(property_c, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            {/* str("c") */
                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "c");
                            }
                        }
                    }
                }
                {/* @itemprop : 1 */
                    CSEM_Property *property_1 = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "1");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_1, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* values */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "1");
                    }
                }
                {/* @itemprop : 2 */
                    CSEM_Property *property_2 = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "2");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_2, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 0);
                }
            }
        }
        {/* root property, 3 : 3 */
            CSEM_Node *node = CSEM_List_Get(children, 1);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_PROPERTY);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Property *property_3 = CSEM_Node_GetObject(node);
            CSEM_List *names = CSEM_Property_GetNames(property_3);
            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "3");

            CSEM_List *values = NULL, *types = NULL;
            CSEM_Property_GetValues(property_3, &values, &types);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
            {/* values */
                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "3");
            }
        }
        {/* root property, 4 : NULL */
            CSEM_Node *node = CSEM_List_Get(children, 2);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_PROPERTY);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Property *property_4 = CSEM_Node_GetObject(node);
            CSEM_List *names = CSEM_Property_GetNames(property_4);
            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "4");

            CSEM_List *values = NULL, *types = NULL;
            CSEM_Property_GetValues(property_4, &values, &types);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
            CU_ASSERT_EQUAL(CSEM_List_Size(types), 0);
        }
    }

FINISH:
    CSEM_URL_Dispose(baseURL);
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_recursive_propvalue() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-propvalue.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                CSEM_Url *id = CSEM_Item_GetId(item);
                CU_ASSERT_EQUAL(id, NULL);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(types, NULL);
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 1);
                {/* @itemprop : address */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* str("77 Massachusetts Ave Cambridge,MA,") */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "77 Massachusetts Ave Cambridge,MA,");
                    }
                    {/* @itemprop : zipcode */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);
                        CSEM_Property *property_b = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Property_GetNames(property_b);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "zipcode");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Property_GetValues(property_b, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                        {/* str("02139") */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "02139");
                        }
                    }
                }
            }
        }
    }
FINISH:
    CSEM_URL_Dispose(baseURL);
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_basic_resolve() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "neil3655");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 9);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* name : Nail */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* band : item */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                    CSEM_Item *item = CSEM_List_Get(values, 0); {
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
                        CU_ASSERT_EQUAL(CSEM_Item_GetTypes(item), NULL);

                        CSEM_List *refs = CSEM_Item_GetRefs(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(refs), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");

                        CSEM_List *properties_band = CSEM_Item_GetProperties(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_band), 3);

                        {/* band-name : Four Parts Water */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 0);
                            names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band-name");

                            CSEM_List *band_values = NULL, *band_types = NULL;
                            CSEM_Property_GetValues(property, &band_values, &band_types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(band_values, 0), "Four \"Parts\" Water");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(band_types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                        {/* city : Boston */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 1);
                            CSEM_List *names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                        {/* state : MA */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 2);
                            CSEM_List *names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                    }
                }
                {/* nationality : British */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 2);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "nationality");
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 1), "home-country");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "British");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* city : Boston */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* state : MA */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* num : +1.617.981.xxxx */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "num");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "+1.617.981.xxxx");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* name : Android */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Android");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* dev : VoIP */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "dev");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "VoIP");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_resolve_recursive_items() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-recursive-itemscope.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL;
    {/* set baseURL */
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 5);
        {/* item A */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item_a = CSEM_Node_GetObject(node);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item_a), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/A");
                free(id);
            }
            {/* types */
                CSEM_List *types_a = CSEM_Item_GetTypes(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(types_a), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_a, 0), "http://sample.org/A");
            }
            {/* refs */
                CSEM_List *refs_a = CSEM_Item_GetRefs(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs_a), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_a, 0), "B");
            }
            {/* properties */
                CSEM_List *properties_a = CSEM_Item_GetProperties(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_a), 1);
                {/* B : ITEM */
                    CSEM_Property *property_a = CSEM_List_Get(properties_a, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_a);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "B");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_a, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    {/* item B */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *item_b = CSEM_List_Get(values, 0);
                        {
                            char *id = NULL;
                            CSEM_URL_Serialize(CSEM_Item_GetId(item_b), &id);
                            CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/B");
                            free(id);
                        }
                        {/* types */
                            CSEM_List *types_b = CSEM_Item_GetTypes(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types_b), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_b, 0), "http://sample.org/B");
                        }
                        {/* refs */
                            CSEM_List *refs_b = CSEM_Item_GetRefs(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(refs_b), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_b, 0), "C");
                        }
                        {/* properties */
                            CSEM_List *properties_b = CSEM_Item_GetProperties(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_b), 1);
                            {/* C : ITEM */
                                CSEM_Property *property_b = CSEM_List_Get(properties_b, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_b);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "C");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_b, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                {/* item C */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                                    CSEM_Item *item_c = CSEM_List_Get(values, 0);
                                    {
                                        char *id = NULL;
                                        CSEM_URL_Serialize(CSEM_Item_GetId(item_c), &id);
                                        CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/C");
                                        free(id);
                                    }
                                    {/* types */
                                        CSEM_List *types_c = CSEM_Item_GetTypes(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(types_c), 1);
                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_c, 0), "http://sample.org/C");
                                    }
                                    {/* refs */
                                        CSEM_List *refs_c = CSEM_Item_GetRefs(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(refs_c), 1);
                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_c, 0), "D");
                                    }
                                    {/* properties */
                                        CSEM_List *properties_c = CSEM_Item_GetProperties(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_c), 1);
                                        {/* D : ITEM */
                                            CSEM_Property *property_c = CSEM_List_Get(properties_c, 0);
                                            CSEM_List *names = CSEM_Property_GetNames(property_c);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "D");

                                            CSEM_List *values = NULL, *types = NULL;
                                            CSEM_Property_GetValues(property_c, &values, &types);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                            {/* item D */
                                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                                                CSEM_Item *item_d = CSEM_List_Get(values, 0);
                                                {
                                                    char *id = NULL;
                                                    CSEM_URL_Serialize(CSEM_Item_GetId(item_d), &id);
                                                    CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/D");
                                                    free(id);
                                                }
                                                {/* types */
                                                    CSEM_List *types_d = CSEM_Item_GetTypes(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(types_d), 1);
                                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_d, 0), "http://sample.org/D");
                                                }
                                                {/* refs */
                                                    CSEM_List *refs_d = CSEM_Item_GetRefs(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(refs_d), 1);
                                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_d, 0), "E");
                                                }
                                                {/* properties */
                                                    CSEM_List *properties_d = CSEM_Item_GetProperties(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(properties_d), 1);
                                                    {/* E : ITEM */
                                                        CSEM_Property *property_d = CSEM_List_Get(properties_d, 0);
                                                        CSEM_List *names = CSEM_Property_GetNames(property_d);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "E");

                                                        CSEM_List *values = NULL, *types = NULL;
                                                        CSEM_Property_GetValues(property_d, &values, &types);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                                        {/* Item E */
                                                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                                                            CSEM_Item *item_e = CSEM_List_Get(values, 0);
                                                            {
                                                                char *id = NULL;
                                                                CSEM_URL_Serialize(CSEM_Item_GetId(item_e), &id);
                                                                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/E");
                                                                free(id);
                                                            }
                                                            {/* types */
                                                                CSEM_List *types_e = CSEM_Item_GetTypes(item_e);
                                                                CU_ASSERT_EQUAL(CSEM_List_Size(types_e), 1);
                                                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_e, 0), "http://sample.org/E");
                                                            }
                                                            {/* refs */
                                                                CSEM_List *refs_e = CSEM_Item_GetRefs(item_e);
                                                                CU_ASSERT_EQUAL(refs_e, NULL);
                                                            }
                                                            {/* properties */
                                                                CSEM_List *properties_e = CSEM_Item_GetProperties(item_e);
                                                                CU_ASSERT_EQUAL(properties_e, NULL);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

FINISH:
    CSEM_URL_Dispose(baseURL);
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_chunked() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    char buf[8];
    size_t readLen = 0;
    while((readLen = read(fd, buf, sizeof(buf)))) {
        if((error = CSEM_Builder_ParseChunk(builder, buf, readLen, 0))) {
            CU_FAIL_FATAL("failed parse");
            goto FINISH;
        }
    }
    if((error = CSEM_Builder_ParseChunk(builder, NULL, 0, 1))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "neil3655");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 9);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* name : Nail */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* band : item */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                    CSEM_Item *item = CSEM_List_Get(values, 0); {
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
                        CU_ASSERT_EQUAL(CSEM_Item_GetTypes(item), NULL);

                        CSEM_List *refs = CSEM_Item_GetRefs(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(refs), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");

                        CSEM_List *properties_band = CSEM_Item_GetProperties(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_band), 3);

                        {/* band-name : Four Parts Water */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 0);
                            names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band-name");

                            CSEM_List *band_values = NULL, *band_types = NULL;
                            CSEM_Property_GetValues(property, &band_values, &band_types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(band_values, 0), "Four \"Parts\" Water");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(band_types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                        {/* city : Boston */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 1);
                            CSEM_List *names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                        {/* state : MA */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 2);
                            CSEM_List *names = CSEM_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        }
                    }
                }
                {/* nationality : British */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 2);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "nationality");
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 1), "home-country");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "British");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* city : Boston */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* state : MA */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* num : +1.617.981.xxxx */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "num");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "+1.617.981.xxxx");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* name : Android */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Android");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* dev : VoIP */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "dev");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "VoIP");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_getItems_no_types() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL; {
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = NULL;
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/neil3655");
                free(id);
            }
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "");
        CSEM_List_Add(types, "");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, NULL);
        CSEM_List_Add(types, NULL);
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, NULL);
        CSEM_List_Add(types, "");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "");
        CSEM_List_Add(types, NULL);
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Book");
        CSEM_List_Add(types, "");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
    CSEM_URL_Dispose(baseURL);
}
void test_microdata_tree_getItems_with_types() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Url *baseURL = NULL; {
        CSEM_URL_Parse("http://localhost/test/", &baseURL);
        CSEM_Builder_SetBaseURL(builder, baseURL);
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Person");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);

            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/neil3655");
                free(id);
            }
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/neil3655");
                free(id);
            }
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/neil3655");
                free(id);
            }
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            {
                char *id = NULL;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/neil3655");
                free(id);
            }
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sampleX.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_List_Add(types, "http://xxx.org");

        CSEM_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 0);
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
    CSEM_URL_Dispose(baseURL);
}
void test_microdata_tree_getNamedProperties() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* search properties whose name is "name" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "name", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 3);
            {/* name : Neil */
                CSEM_Property *property = CSEM_List_Get(properties, 0);
                CSEM_List *names = CSEM_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Property_GetValues(property, &values, &valueTypes);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                }
            }
            {/* name : Nail */
                CSEM_Property *property = CSEM_List_Get(properties, 1);
                CSEM_List *names = CSEM_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Property_GetValues(property, &values, &valueTypes);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                }
            }
            {/* name : Android */
                CSEM_Property *property = CSEM_List_Get(properties, 2);
                CSEM_List *names = CSEM_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Property_GetValues(property, &values, &valueTypes);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Android");
                }
            }
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
    }
    {/* search properties whose name is "name" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "band", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 1);
            {/* band : ITEM */
                CSEM_Property *property = CSEM_List_Get(properties, 0);
                CSEM_List *names = CSEM_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Property_GetValues(property, &values, &valueTypes);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(valueTypes, 0)), CSEM_VALUE_TYPE_ITEM);
                    CSEM_Item *item_band = CSEM_List_Get(values, 0);
                    CSEM_List *refs = CSEM_Item_GetRefs(item_band);
                    CU_ASSERT_EQUAL(CSEM_List_Size(refs), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                }
            }
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
    }
    {/* search properties whose name is "" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 0);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_getNamedProperties_no_name() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* search properties whose name is NULL */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, NULL, &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 0);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
    }
    {/* search properties whose name is "" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 0);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_properties_getValues() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/micro-basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* search properties whose name is "name" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "name", &properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Properties_GetValues(properties, &values, &valueTypes);

            CU_ASSERT_EQUAL(CSEM_List_Size(values), 3)
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 1), "Nail");
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 2), "Android");
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
        CSEM_List_Dispose(values, CSEM_FALSE);
        CSEM_List_Dispose(valueTypes, CSEM_FALSE);
    }
    {/* search child item's properties whose name is "city" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *top_properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "band", &top_properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Properties_GetValues(top_properties, &values, &valueTypes);

            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1)
            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(valueTypes, 0)), CSEM_VALUE_TYPE_ITEM);

            CSEM_Item *sub_item = CSEM_List_Get(values, 0);
            CSEM_List *sub_properties = NULL;
            CSEM_List *sub_values = NULL, *sub_valueTypes = NULL; {
                CSEM_Item_GetNamedPropertes(sub_item, "city", &sub_properties);
                CSEM_Properties_GetValues(sub_properties, &sub_values, &sub_valueTypes);
                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(sub_valueTypes, 0)), CSEM_VALUE_TYPE_STR);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(sub_values, 0), "Boston");
            }
            /* dispose the search result item */
            CSEM_List_Dispose(sub_properties, CSEM_FALSE);
            CSEM_List_Dispose(sub_values, CSEM_FALSE);
            CSEM_List_Dispose(sub_valueTypes, CSEM_FALSE);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(top_properties, CSEM_FALSE);
        CSEM_List_Dispose(values, CSEM_FALSE);
        CSEM_List_Dispose(valueTypes, CSEM_FALSE);
    }
    {/* search properties whose name is "NONE" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Item_GetNamedPropertes(item, "NONE", &properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Properties_GetValues(properties, &values, &valueTypes);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
        CSEM_List_Dispose(values, CSEM_FALSE);
        CSEM_List_Dispose(valueTypes, CSEM_FALSE);
    }
    {/* properties = NULL */
        CSEM_List *properties = NULL;
        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Properties_GetValues(properties, &values, &valueTypes);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
        CSEM_List_Dispose(values, CSEM_FALSE);
        CSEM_List_Dispose(valueTypes, CSEM_FALSE);
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_schema_org_event_ex1() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-event-ex1.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Event");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 4);

                {/* url : */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);

                    {/* url : nba-miami-philidelphia-game3.html */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "nba-miami-philidelphia-game3.html");
                    }
                    {/* name : Miami ... */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);
                        CSEM_Property *tmpProperty = CSEM_List_Get(values, 1);

                        CSEM_List *tmpNames = CSEM_Property_GetNames(tmpProperty);
                        CU_ASSERT_EQUAL(CSEM_List_Size(tmpNames), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tmpNames, 0), "name");

                        CSEM_List *tmpValues = NULL, *tmpValueTypes = NULL; {
                            CSEM_Property_GetValues(tmpProperty, &tmpValues, &tmpValueTypes);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpValues), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tmpValues, 0), " Miami Heat at Philadelphia 76ers - Game 3 (Home Game 1) ");
                        }
                    }
                }
                {/* startDate : 2016-04-21T20:00 */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "startDate");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "2016-04-21T20:00");
                    }
                }
                {/* location : */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "location");

                    CSEM_List *values = NULL, *types = NULL; {

                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                    }
                }
                {/* offers : */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "offers");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                    }
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_schema_org_event_ex2() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-event-ex2.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);

        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/MusicGroup");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 10);

                {/* name : */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Foo Fighters");
                }
                {/* video : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        /* ITEM */
                        CSEM_Item *tmpItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(tmpItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/VideoObject");
                        }
                        {/* refs */
                            CSEM_List *refs = CSEM_Item_GetRefs(tmpItem);
                            CU_ASSERT_EQUAL(refs, NULL);
                        }
                        {/* properties */
                            CSEM_List *tmpProperties = CSEM_Item_GetProperties(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpProperties), 4);

                            {/* name : Interview with the Foo Fighters */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Interview with the Foo Fighters");
                            }
                            {/* duration : T1M33S */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "duration");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "T1M33S");
                            }
                            {/* thumbnail : foo-fighters-interview-thumb.jpg */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 2);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "thumbnail");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foo-fighters-interview-thumb.jpg");
                            }
                            {/* description : ... */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 3);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "description");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            }
                        }
                    }
                }
                {/* track : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "track");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        /* ITEM */
                        CSEM_Item *tmpItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(tmpItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/MusicRecording");
                        }
                        {/* refs */
                            CSEM_List *refs = CSEM_Item_GetRefs(tmpItem);
                            CU_ASSERT_EQUAL(refs, NULL);
                        }
                        {/* properties */
                            CSEM_List *tmpProperties = CSEM_Item_GetProperties(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpProperties), 7);

                            {/* name : Rope */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Rope");
                            }
                            {/* url : foo-fighters-rope.html */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foo-fighters-rope.html");
                            }
                            {/* duration : PT4M5S */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 2);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "duration");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "PT4M5S");
                            }
                            {/* interactionCount : UserPlays:14300 */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 3);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "interactionCount");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "UserPlays:14300");
                            }
                            {/* audio : foo-fighters-rope-play.html */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 4);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "audio");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foo-fighters-rope-play.html");
                            }
                            {/* offers : foo-fighters-rope-buy.html */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 5);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "offers");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foo-fighters-rope-buy.html");
                            }
                            {/* inAlbum : foo-fighters-wasting-light.html */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 6);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "inAlbum");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foo-fighters-wasting-light.html");
                            }
                        }
                    }
                }
                {/* track : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "track");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);

                        //TODO
                    }
                }
                {/* event : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "event");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);

                        //TODO
                    }
                }
                {/* event : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "event");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);

                        //TODO
                    }
                }
                {/* image : foofighters-1.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "image");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foofighters-1.jpg");
                    }
                }
                {/* image : foofighters-2.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "image");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foofighters-2.jpg");
                    }
                }
                {/* image : foofighters-3.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "image");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "foofighters-3.jpg");
                    }
                }
                {/* interactionCount : UserComments:18 */
                    CSEM_Property *property = CSEM_List_Get(properties, 9);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "interactionCount");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "UserComments:18");
                    }
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_schema_org_person() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-person.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);

        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 9);

                {/* name : Jane Doe */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Jane Doe");
                }
                {/* image : janedoe.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "image");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "janedoe.jpg");
                }
                {/* jobTitle : Professor */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "jobTitle");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Professor");
                }
                {/* address : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        /* ITEM */
                        CSEM_Item *tmpItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(tmpItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/PostalAddress");
                        }
                        {/* refs */
                            CSEM_List *refs = CSEM_Item_GetRefs(tmpItem);
                            CU_ASSERT_EQUAL(refs, NULL);
                        }
                        {/* properties */
                            CSEM_List *tmpProperties = CSEM_Item_GetProperties(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpProperties), 4);

                            {/* streetAddress : ... */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "streetAddress");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            }
                            {/* addressLocality : Seattle */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressLocality");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Seattle");
                            }
                            {/* addressRegion : WA */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 2);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressRegion");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "WA");
                            }
                            {/* postalCode : 98052 */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 3);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "postalCode");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "98052");
                            }
                        }
                    }
                }
                {/* telephone : (425) 123-4567 */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "telephone");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "(425) 123-4567");
                }
                {/* email : mailto:jane-doe@xyz.edu */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "email");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "mailto:jane-doe@xyz.edu");
                }
                {/* url : http://www.janedoe.com */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "http://www.janedoe.com");
                }
                {/* colleague : http://www.xyz.edu/students/alicejones.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "colleague");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "http://www.xyz.edu/students/alicejones.html");
                }
                {/* colleague : http://www.xyz.edu/students/bobsmith.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "colleague");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "http://www.xyz.edu/students/bobsmith.html");
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_schema_org_place_ex1() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-place-ex1.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);

        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/LocalBusiness");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 4);

                {/* name */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Beachwalk Beachwear & Giftware");
                }
                {/* description */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "description");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                }
                {/* address : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        /* ITEM */
                        CSEM_Item *tmpItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(tmpItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/PostalAddress");
                        }
                        {/* refs */
                            CSEM_List *refs = CSEM_Item_GetRefs(tmpItem);
                            CU_ASSERT_EQUAL(refs, NULL);
                        }
                        {/* properties */
                            CSEM_List *tmpProperties = CSEM_Item_GetProperties(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpProperties), 3);

                            {/* streetAddress : 3102 Highway 98 */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "streetAddress");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "3102 Highway 98");
                            }
                            {/* addressLocality : Mexico Beach */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressLocality");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Mexico Beach");
                            }
                            {/* addressRegion : FL */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 2);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressRegion");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "FL");
                            }
                        }
                    }
                }
                {/* telephone : 850-648-4200 */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "telephone");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "850-648-4200");
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);

}
void test_microdata_tree_schema_org_place_ex2() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-place-ex2.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);

        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Place");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 2);

                {/* name : Empire State Building */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Empire State Building");
                }
                {/* geo : ITEM */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "geo");

                    CSEM_List *values = NULL, *types = NULL; {
                        CSEM_Property_GetValues(property, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        /* ITEM */
                        CSEM_Item *tmpItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(tmpItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/GeoCoordinates");
                        }
                        {/* refs */
                            CSEM_List *refs = CSEM_Item_GetRefs(tmpItem);
                            CU_ASSERT_EQUAL(refs, NULL);
                        }
                        {/* properties */
                            CSEM_List *tmpProperties = CSEM_Item_GetProperties(tmpItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(tmpProperties), 2);

                            {/* latitude : 40.75 */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "latitude");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "40.75");
                            }
                            {/* longitude : 73.98 */
                                CSEM_Property *property = CSEM_List_Get(tmpProperties, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "longitude");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "73.98");
                            }
                        }
                    }
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_schema_org_place_ex4() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/schema-org-place-ex4.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);

        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/CivicStructure");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 3);

                {/* name : Springfield Town Hall */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Springfield Town Hall");
                }
                {/* openingHours : Mo-Fr 09:00-17:30 */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "openingHours");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Mo-Fr 09:00-17:30");
                }
                {/* openingHours : Sa 09:00-12:00 */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "openingHours");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Sa 09:00-12:00");
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
