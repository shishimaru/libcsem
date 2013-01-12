#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

#include "csem_utils.h"
#include "test_micro_tree.h"

void test_microdata_tree_basic_no_resolve() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_FALSE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
            {/* types */
                CSEM_List *types = CSEM_Micro_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Micro_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 4);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* name : Nail */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* band : item */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                    CSEM_Item *item = CSEM_List_Get(values, 0); {
                        CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
                        CU_ASSERT_EQUAL(CSEM_Micro_Item_GetTypes(item), NULL);
                        CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(refs), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");

                        CSEM_List *properties_band = CSEM_Micro_Item_GetProperties(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_band), 1);

                        {/* band-name : Four Parts Water */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 0);
                            names = CSEM_Micro_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band-name");

                            CSEM_List *band_values = NULL, *band_types = NULL;
                            CSEM_Micro_Property_GetValues(property, &band_values, &band_types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(band_values, 0), "Four Parts Water");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(band_types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        }
                    }
                }
                {/* nationality : British */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 2);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "nationality");
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 1), "home-country");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "British");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
            }
        }{/* 1st id */
            CSEM_Node *node = CSEM_List_Get(children, 1);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ID);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Id *id = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Id_GetId(id), "address");
            {/* properties */
                CSEM_List *properties = CSEM_Micro_Id_GetProperties(id);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 2);
                {/* city : Boston */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* state : MA */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
            }
        }{/* 2nd id */
            CSEM_Node *node = CSEM_List_Get(children, 2);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ID);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Id *id = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Id_GetId(id), "phone");
            {/* properties */
                CSEM_List *properties = CSEM_Micro_Id_GetProperties(id);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 3);
                {/* num : +1.617.981.xxxx */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "num");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "+1.617.981.xxxx");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* name : Android */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Android");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* dev : VoIP */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "dev");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "VoIP");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_values() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/5-4-Values.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_FALSE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 1);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
            {/* types */
                CSEM_List *types = CSEM_Micro_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Micro_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 28);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* name : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* music : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mp3");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* movie : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.swf");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* frame : sample.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "frame");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* frame : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "frame");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* photo : sample.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.jpg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* photo : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 9);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* music : sample.mp3 */
                    CSEM_Property *property = CSEM_List_Get(properties, 10);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mp3");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* music : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 11);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "music");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* video : sample.m4v */
                    CSEM_Property *property = CSEM_List_Get(properties, 12);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.m4v");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* video : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 13);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* video : sample.vtt */
                    CSEM_Property *property = CSEM_List_Get(properties, 14);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.vtt");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* video : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 15);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "video");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* movie : sample.mpeg */
                    CSEM_Property *property = CSEM_List_Get(properties, 16);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.mpeg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* movie : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 17);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "movie");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* licence : sample.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 18);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "licence");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "licence.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* licence : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 19);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "licence");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* top : index.html */
                    CSEM_Property *property = CSEM_List_Get(properties, 20);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "top");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "index.html");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* top : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 21);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "top");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* photo : sample.jpg */
                    CSEM_Property *property = CSEM_List_Get(properties, 22);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.jpg");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* photo : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 23);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "photo");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_URL);
                }
                {/* animation : sample.gif */
                    CSEM_Property *property = CSEM_List_Get(properties, 24);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "animation");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "sample.gif");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* animation : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 25);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "animation");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* updated : 2000-01-01 */
                    CSEM_Property *property = CSEM_List_Get(properties, 26);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "updated");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "2000-01-01");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* updated : NONE */
                    CSEM_Property *property = CSEM_List_Get(properties, 27);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "updated");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
            }
        }
    }

FINISH:
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
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
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
    if(!(fd = open("./data/recursive-itemprop.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_FALSE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "root");
            {/* types */
                CSEM_List *types = CSEM_Micro_Item_GetTypes(item);
                CU_ASSERT_EQUAL(types, NULL);
            }
            {/* refs */
                CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                CU_ASSERT_EQUAL(refs, NULL);
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Micro_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 3);
                {/* @itemprop : A */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "A");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* str("a") */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "a");
                    }
                    {/* @itemprop : B */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_MICRO_VALUE_TYPE_PROPERTY);
                        CSEM_Property *property_b = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Micro_Property_GetNames(property_b);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "B");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Micro_Property_GetValues(property_b, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                        {/* str("b") */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "b");
                        }
                        {/* @itemprop : C */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_MICRO_VALUE_TYPE_PROPERTY);
                            CSEM_Property *property_c = CSEM_List_Get(values, 1);
                            CSEM_List *names = CSEM_Micro_Property_GetNames(property_c);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "C");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Micro_Property_GetValues(property_c, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            {/* str("c") */
                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "c");
                            }
                        }
                    }
                }
                {/* @itemprop : 1 */
                    CSEM_Property *property_1 = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property_1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "1");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property_1, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* values */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "1");
                    }
                }
                {/* @itemprop : 2 */
                    CSEM_Property *property_2 = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property_2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "2");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property_2, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 0);
                }
            }
        }
        {/* root property, 3 : 3 */
            CSEM_Node *node = CSEM_List_Get(children, 1);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_PROPERTY);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Property *property_3 = CSEM_Node_GetObject(node);
            CSEM_List *names = CSEM_Micro_Property_GetNames(property_3);
            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "3");

            CSEM_List *values = NULL, *types = NULL;
            CSEM_Micro_Property_GetValues(property_3, &values, &types);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
            {/* values */
                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "3");
            }
        }
        {/* root property, 4 : NULL */
            CSEM_Node *node = CSEM_List_Get(children, 2);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_PROPERTY);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Property *property_4 = CSEM_Node_GetObject(node);
            CSEM_List *names = CSEM_Micro_Property_GetNames(property_4);
            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "4");

            CSEM_List *values = NULL, *types = NULL;
            CSEM_Micro_Property_GetValues(property_4, &values, &types);
            CU_ASSERT_EQUAL(CSEM_List_Size(values), 0);
            CU_ASSERT_EQUAL(CSEM_List_Size(types), 0);
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_basic_resolve() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 3);
        {/* 1st item */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
            {/* types */
                CSEM_List *types = CSEM_Micro_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://sample2.org/Person");
            }
            {/* refs */
                CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 1), "phone");
            }
            {/* properties */
                CSEM_List *properties = CSEM_Micro_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties), 9);
                {/* name : Neil */
                    CSEM_Property *property = CSEM_List_Get(properties, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* name : Nail */
                    CSEM_Property *property = CSEM_List_Get(properties, 1);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* band : item */
                    CSEM_Property *property = CSEM_List_Get(properties, 2);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                    CSEM_Item *item = CSEM_List_Get(values, 0); {
                        CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
                        CU_ASSERT_EQUAL(CSEM_Micro_Item_GetTypes(item), NULL);

                        CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(refs), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs, 0), "address");

                        CSEM_List *properties_band = CSEM_Micro_Item_GetProperties(item);
                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_band), 3);

                        {/* band-name : Four Parts Water */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 0);
                            names = CSEM_Micro_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band-name");

                            CSEM_List *band_values = NULL, *band_types = NULL;
                            CSEM_Micro_Property_GetValues(property, &band_values, &band_types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(band_types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(band_values, 0), "Four Parts Water");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(band_types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        }
                        {/* city : Boston */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 1);
                            CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Micro_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        }
                        {/* state : MA */
                            CSEM_Property *property = CSEM_List_Get(properties_band, 2);
                            CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                            CSEM_List *values = NULL, *types = NULL;
                            CSEM_Micro_Property_GetValues(property, &values, &types);
                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                        }
                    }
                }
                {/* nationality : British */
                    CSEM_Property *property = CSEM_List_Get(properties, 3);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 2);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "nationality");
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 1), "home-country");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "British");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* city : Boston */
                    CSEM_Property *property = CSEM_List_Get(properties, 4);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "city");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* state : MA */
                    CSEM_Property *property = CSEM_List_Get(properties, 5);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "state");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "MA");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* num : +1.617.981.xxxx */
                    CSEM_Property *property = CSEM_List_Get(properties, 6);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "num");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "+1.617.981.xxxx");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* name : Android */
                    CSEM_Property *property = CSEM_List_Get(properties, 7);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Android");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
                }
                {/* dev : VoIP */
                    CSEM_Property *property = CSEM_List_Get(properties, 8);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "dev");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "VoIP");
                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_STR);
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
    if(!(fd = open("./data/recursive-itemscope.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *children = CSEM_Document_GetChildren(doc);
        CU_ASSERT_EQUAL(CSEM_List_Size(children), 5);
        {/* item A */
            CSEM_Node *node = CSEM_List_Get(children, 0);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_MICRO_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item_a = CSEM_Node_GetObject(node);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item_a), "A");
            {/* types */
                CSEM_List *types_a = CSEM_Micro_Item_GetTypes(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(types_a), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_a, 0), "http://sample.org/A");
            }
            {/* refs */
                CSEM_List *refs_a = CSEM_Micro_Item_GetRefs(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(refs_a), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_a, 0), "B");
            }
            {/* properties */
                CSEM_List *properties_a = CSEM_Micro_Item_GetProperties(item_a);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_a), 1);
                {/* B : ITEM */
                    CSEM_Property *property_a = CSEM_List_Get(properties_a, 0);
                    CSEM_List *names = CSEM_Micro_Property_GetNames(property_a);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "B");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Micro_Property_GetValues(property_a, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                    {/* item B */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                        CSEM_Item *item_b = CSEM_List_Get(values, 0);
                        CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item_b), "B");
                        {/* types */
                            CSEM_List *types_b = CSEM_Micro_Item_GetTypes(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types_b), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_b, 0), "http://sample.org/B");
                        }
                        {/* refs */
                            CSEM_List *refs_b = CSEM_Micro_Item_GetRefs(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(refs_b), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_b, 0), "C");
                        }
                        {/* properties */
                            CSEM_List *properties_b = CSEM_Micro_Item_GetProperties(item_b);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_b), 1);
                            {/* C : ITEM */
                                CSEM_Property *property_b = CSEM_List_Get(properties_b, 0);
                                CSEM_List *names = CSEM_Micro_Property_GetNames(property_b);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "C");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Micro_Property_GetValues(property_b, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                {/* item C */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                                    CSEM_Item *item_c = CSEM_List_Get(values, 0);
                                    CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item_c), "C");
                                    {/* types */
                                        CSEM_List *types_c = CSEM_Micro_Item_GetTypes(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(types_c), 1);
                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_c, 0), "http://sample.org/C");
                                    }
                                    {/* refs */
                                        CSEM_List *refs_c = CSEM_Micro_Item_GetRefs(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(refs_c), 1);
                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_c, 0), "D");
                                    }
                                    {/* properties */
                                        CSEM_List *properties_c = CSEM_Micro_Item_GetProperties(item_c);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_c), 1);
                                        {/* D : ITEM */
                                            CSEM_Property *property_c = CSEM_List_Get(properties_c, 0);
                                            CSEM_List *names = CSEM_Micro_Property_GetNames(property_c);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "D");

                                            CSEM_List *values = NULL, *types = NULL;
                                            CSEM_Micro_Property_GetValues(property_c, &values, &types);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                            {/* item D */
                                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                                                CSEM_Item *item_d = CSEM_List_Get(values, 0);
                                                CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item_d), "D");
                                                {/* types */
                                                    CSEM_List *types_d = CSEM_Micro_Item_GetTypes(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(types_d), 1);
                                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_d, 0), "http://sample.org/D");
                                                }
                                                {/* refs */
                                                    CSEM_List *refs_d = CSEM_Micro_Item_GetRefs(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(refs_d), 1);
                                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(refs_d, 0), "E");
                                                }
                                                {/* properties */
                                                    CSEM_List *properties_d = CSEM_Micro_Item_GetProperties(item_d);
                                                    CU_ASSERT_EQUAL(CSEM_List_Size(properties_d), 1);
                                                    {/* E : ITEM */
                                                        CSEM_Property *property_d = CSEM_List_Get(properties_d, 0);
                                                        CSEM_List *names = CSEM_Micro_Property_GetNames(property_d);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "E");

                                                        CSEM_List *values = NULL, *types = NULL;
                                                        CSEM_Micro_Property_GetValues(property_d, &values, &types);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);

                                                        {/* Item E */
                                                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                                                            CSEM_Item *item_e = CSEM_List_Get(values, 0);
                                                            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item_e), "E");
                                                            {/* types */
                                                                CSEM_List *types_e = CSEM_Micro_Item_GetTypes(item_e);
                                                                CU_ASSERT_EQUAL(CSEM_List_Size(types_e), 1);
                                                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types_e, 0), "http://sample.org/E");
                                                            }
                                                            {/* refs */
                                                                CSEM_List *refs_e = CSEM_Micro_Item_GetRefs(item_e);
                                                                CU_ASSERT_EQUAL(refs_e, NULL);
                                                            }
                                                            {/* properties */
                                                                CSEM_List *properties_e = CSEM_Micro_Item_GetProperties(item_e);
                                                                CU_ASSERT_EQUAL(CSEM_List_Size(properties_e), 0);
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
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_getItems_no_types() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = NULL;
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_List_Add(types, "");
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_List_Add(types, NULL);
        CSEM_List_Add(types, NULL);
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_List_Add(types, NULL);
        CSEM_List_Add(types, "");
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_List_Add(types, NULL);
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
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
        CSEM_List_Add(types, "http://sample.org/Book");
        CSEM_List_Add(types, "");
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 2);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* 2nd item */
            CSEM_Item *item = CSEM_List_Get(items, 1);
            CU_ASSERT_EQUAL(CSEM_Micro_Item_GetId(item), NULL);
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_getItems_with_types() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }

    {/* check results */
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, "http://sample.org/Person");
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
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
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
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
        CSEM_List_Add(types, "http://sample1.org/Person");
        CSEM_List_Add(types, "http://sample2.org/Person");
        CSEM_Micro_Document_GetItems(doc, types, &items);

        CU_ASSERT_EQUAL(CSEM_List_Size(items), 1);
        {/* 1st item */
            CSEM_Item *item = CSEM_List_Get(items, 0);
            CU_ASSERT_STRING_EQUAL(CSEM_Micro_Item_GetId(item), "urn:sample:0001");
        }
        {/* dispose temporaries */
            CSEM_List_Dispose(types, CSEM_FALSE);
            /* dispose the search result item */
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
    }
FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
void test_microdata_tree_getNamedProperties() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }

    {/* search properties whose name is "name" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Micro_Item_GetNamedPropertes(item, "name", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 3);
            {/* name : Neil */
                CSEM_Property *property = CSEM_List_Get(properties, 0);
                CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &valueTypes);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                }
            }
            {/* name : Nail */
                CSEM_Property *property = CSEM_List_Get(properties, 1);
                CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &valueTypes);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                }
            }
            {/* name : Android */
                CSEM_Property *property = CSEM_List_Get(properties, 2);
                CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &valueTypes);
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
        CSEM_Micro_Item_GetNamedPropertes(item, "band", &properties);

        {/* check results */
            CU_ASSERT_EQUAL(CSEM_List_Size(properties), 1);
            {/* band : ITEM */
                CSEM_Property *property = CSEM_List_Get(properties, 0);
                CSEM_List *names = CSEM_Micro_Property_GetNames(property);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "band");

                {/* check values */
                    CSEM_List *values = NULL, *valueTypes = NULL;
                    CSEM_Micro_Property_GetValues(property, &values, &valueTypes);

                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(valueTypes, 0)), CSEM_MICRO_VALUE_TYPE_ITEM);
                    CSEM_Item *item_band = CSEM_List_Get(values, 0);
                    CSEM_List *refs = CSEM_Micro_Item_GetRefs(item_band);
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
        CSEM_Micro_Item_GetNamedPropertes(item, "", &properties);

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
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* search properties whose name is NULL */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Micro_Item_GetNamedPropertes(item, NULL, &properties);

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
        CSEM_Micro_Item_GetNamedPropertes(item, "", &properties);

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
    if(!(fd = open("./data/basic.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    {/* search properties whose name is "name" */
        CSEM_Item *item = NULL; {
            CSEM_List *nodes = CSEM_Document_GetChildren(doc);
            CSEM_Node *node = CSEM_List_Get(nodes, 0);
            item = CSEM_Node_GetObject(node);
        }
        CSEM_List *properties = NULL;
        CSEM_Micro_Item_GetNamedPropertes(item, "name", &properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Micro_Properties_GetValues(properties, &values, &valueTypes);

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
    {/* search 2nd item's properties whose name is "city" */
        CSEM_Item *item = NULL; {
            CSEM_List *items = NULL;
            CSEM_Micro_Document_GetItems(doc, NULL, &items);
            item = CSEM_List_Get(items, 1);
            CSEM_List_Dispose(items, CSEM_FALSE);
        }
        CSEM_List *properties = NULL;
        CSEM_Micro_Item_GetNamedPropertes(item, "city", &properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Micro_Properties_GetValues(properties, &values, &valueTypes);

            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1)
            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Boston");
        }
        /* dispose the search result item */
        CSEM_List_Dispose(properties, CSEM_FALSE);
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
        CSEM_Micro_Item_GetNamedPropertes(item, "NONE", &properties);

        CSEM_List *values = NULL, *valueTypes = NULL;
        {/* check results */
            CSEM_Micro_Properties_GetValues(properties, &values, &valueTypes);
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
            CSEM_Micro_Properties_GetValues(properties, &values, &valueTypes);
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
