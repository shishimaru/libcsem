#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <CUnit/CUnit.h>
#include "csem/csem_builder.h"
#include "csem/csem_rdfa_tree.h"

#include "csem_utils.h"
#include "test_micro_tree.h"

void test_microdata_tree_no_rdfa() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/no-rdfa.html", O_RDONLY))) {
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
void test_rdfa_tree_basic() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-basic.html", O_RDONLY))) {
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
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/main");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Person");
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://schema.org/Person");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 5);
                {/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : Neil */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Neil");
                    }
                }{/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : Nail */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Nail");
                    }
                }{/* @property = "jobTitle" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "jobTitle");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        {
                            char *id;
                            CSEM_URL_Serialize(CSEM_Item_GetId(subItem), &id);
                            CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/sub1");
                            free(id);
                        }
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema2.org/MusicRecording");
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://schema2.org/MusicRecording");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 1);
                            {/* @property = "byArtist" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "byArtist");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* byArtist : Neil */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Four \"Parts\" Water");
                                }
                            }
                        }
                    }
                }{/* @property = "jobTitle" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "jobTitle");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        {
                            char *id;
                            CSEM_URL_Serialize(CSEM_Item_GetId(subItem), &id);
                            CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/sub2");
                            free(id);
                        }
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/MusicRecording");
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://schema2.org/MusicRecording");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 1);
                            {/* @property = "byArtist" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "byArtist");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* byArtist : Neil */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Four \"Parts\" Water");
                                }
                            }
                        }
                    }
                }{/* @property = "address" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(subItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Person");
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 1), "http://schema.org/Person");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 1);
                            {/* @property = "homeLocation" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "homeLocation");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* homeLocation : British */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "British");
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
void test_rdfa_tree_recursive_propvalue() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-propvalue.html", O_RDONLY))) {
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
                CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://sample.org/Address");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 1);
                {/* @property = "address" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* name : Neil */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "77 Massachusetts Ave Cambridge,MA,");
                    }
                    {/* @property = "zipcode" */
                        CSEM_Property *property_sub = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Property_GetNames(property_sub);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "zipcode");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Property_GetValues(property_sub, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                        {/* name : Nail */
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
void test_rdfa_sample_person() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-person.html", O_RDONLY))) {
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
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Person");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 5);
                {/* @property = "image" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "image");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* image : http://manu.sporny.org/images/manu.png */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "http://manu.sporny.org/images/manu.png");
                    }
                    {/* @property : name */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);
                        CSEM_Property *property_sub = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Property_GetNames(property_sub);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Property_GetValues(property_sub, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                        {/* name : Manu Sporny */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Manu Sporny");
                        }
                    }
                }{/* @property = "jobTitle" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "jobTitle");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* jobTitle : Founder/CEO */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Founder/CEO");
                    }
                }{/* @property = "telephone" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "telephone");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* telephone : (540) 961-4469 */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "(540) 961-4469");
                    }
                }{/* @property = "email" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "email");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* email : mailto:msporny@digitalbazaar.com */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "mailto:msporny@digitalbazaar.com");
                    }
                }{/* @property = "url" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 4);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* url : http://manu.sporny.org/ */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "http://manu.sporny.org/");
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
void test_rdfa_sample_ssn() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-ssn.html", O_RDONLY))) {
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
                char *id;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/#manu");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://xmlns.com/foaf/0.1/Person");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 3);
                {/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : Manu Sporny */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Manu Sporny");
                    }
                }{/* @property = "knows" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "knows");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* knows : #alex */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "#alex");
                    }
                }{/* @property = "knows" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "knows");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* knows : #alex */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "#brian");
                    }
                }
            }
        }{/* 2nd item */
            CSEM_Node *node = CSEM_List_Get(children, 1);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/#alex");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://xmlns.com/foaf/0.1/Person");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 1);
                {/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : Alex Milowski */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Alex Milowski");
                    }
                }
            }
        }{/* 3rd item */
            CSEM_Node *node = CSEM_List_Get(children, 2);
            CU_ASSERT_EQUAL(CSEM_Node_GetType(node), CSEM_NODE_TYPE_ITEM);
            CU_ASSERT_PTR_EQUAL(CSEM_Node_GetObject(CSEM_Node_GetParent(node)), doc);

            CSEM_Item *item = CSEM_Node_GetObject(node);
            {
                char *id;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/#brian");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://xmlns.com/foaf/0.1/Person");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 1);
                {/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : Brian Sletten */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Brian Sletten");
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
void test_rdfa_sample_event() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-event.html", O_RDONLY))) {
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
            CU_ASSERT_EQUAL(CSEM_Item_GetId(item), NULL);
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Event");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 4);
                {/* @property = "url" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 2);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 2);
                    {/* url : "nba-miami-philidelphia-game3.html */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "nba-miami-philidelphia-game3.html");
                    }
                    {/* @property = "name" */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 1)), CSEM_VALUE_TYPE_PROPERTY);

                        CSEM_Property *property_sub = CSEM_List_Get(values, 1);
                        CSEM_List *names = CSEM_Property_GetNames(property_sub);
                        CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                        CSEM_List *values = NULL, *types = NULL;
                        CSEM_Property_GetValues(property_sub, &values, &types);
                        CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                        {/* name : Miami Heat at Philadelphia 76ers - Game 3 (Home Game 1) */
                            CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Miami Heat at Philadelphia 76ers - Game 3 (Home Game 1)");
                        }
                    }
                }{/* @property = "startDate" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "startDate");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* startDate : 2011-04-21T20:00 */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "2011-04-21T20:00");
                    }
                }{/* @property = "location" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "location");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(subItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/Place");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 2);
                            {/* @property = "url" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "url");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* url : wells-fargo-center.html */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_URL);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "wells-fargo-center.html");
                                }
                            }
                            {/* @property = "address" */
                                CSEM_Property *property_root = CSEM_List_Get(properties_sub, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property_root);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_root, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* Item */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                                    CSEM_Item *subsubItem = CSEM_List_Get(values, 0);
                                    CU_ASSERT_EQUAL(CSEM_Item_GetId(subsubItem), NULL);
                                    {/* types */
                                        CSEM_List *types = CSEM_Item_GetTypes(subsubItem);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/PostalAddress");
                                    }
                                    {/* properties */
                                        CSEM_List *properties_sub = CSEM_Item_GetProperties(subsubItem);
                                        CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 2);
                                        {/* @property = "addressLocality" */
                                            CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                            CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressLocality");

                                            CSEM_List *values = NULL, *types = NULL;
                                            CSEM_Property_GetValues(property_sub, &values, &types);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                            {/* addressLocality : Philadelphia */
                                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Philadelphia");
                                            }
                                        }
                                        {/* @property = "addressLocality" */
                                            CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 1);
                                            CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressRegion");

                                            CSEM_List *values = NULL, *types = NULL;
                                            CSEM_Property_GetValues(property_sub, &values, &types);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                            {/* addressRegion : PA */
                                                CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "PA");
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }{/* @property = "offers" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "offers");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        CU_ASSERT_EQUAL(CSEM_Item_GetId(subItem), NULL);
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/AggregateOffer");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 2);
                            {/* @property = "lowPrice" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "lowPrice");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* lowPrice : $35 */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "$35");
                                }
                            }
                            {/* @property = "offerCount" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "offerCount");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* offerCount : 1,938 */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "1,938");
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
void test_rdfa_sample_place() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/rdfa-lite-place.html", O_RDONLY))) {
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
            {/* resource */
                char *id;
                CSEM_URL_Serialize(CSEM_Item_GetId(item), &id);
                CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/#bbg");
                free(id);
            }
            {/* types */
                CSEM_List *types = CSEM_Item_GetTypes(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/LocalBusiness");
            }
            {/* properties */
                CSEM_List *properties_root = CSEM_Item_GetProperties(item);
                CU_ASSERT_EQUAL(CSEM_List_Size(properties_root), 4);
                {/* @property = "name" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 0);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "name");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* name : "Beachwalk Beachwear &amp; Giftware" */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Beachwalk Beachwear & Giftware");
                    }
                }{/* @property = "description" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 1);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "description");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* description */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0),
                                " A superb collection of fine gifts and "
                                "clothing\n  to accent your stay in Mexico Beach.");
                    }
                }{/* @property = "address" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 2);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "address");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* Item */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_ITEM);
                        CSEM_Item *subItem = CSEM_List_Get(values, 0);
                        {/* resource */
                            char *id;
                            CSEM_URL_Serialize(CSEM_Item_GetId(subItem), &id);
                            CU_ASSERT_STRING_EQUAL(id, "http://localhost/test/#bbg-address");
                            free(id);
                        }
                        {/* types */
                            CSEM_List *types = CSEM_Item_GetTypes(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                            CU_ASSERT_STRING_EQUAL(CSEM_List_Get(types, 0), "http://schema.org/PostalAddress");
                        }
                        {/* properties */
                            CSEM_List *properties_sub = CSEM_Item_GetProperties(subItem);
                            CU_ASSERT_EQUAL(CSEM_List_Size(properties_sub), 3);
                            {/* @property = "streetAddress" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 0);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "streetAddress");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* streetAddress : 3102 Highway 98 */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "3102 Highway 98");
                                }
                            }
                            {/* @property = "addressLocality" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 1);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressLocality");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* addressLocality : Mexico Beach */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "Mexico Beach");
                                }
                            }
                            {/* @property = "addressRegion" */
                                CSEM_Property *property_sub = CSEM_List_Get(properties_sub, 2);
                                CSEM_List *names = CSEM_Property_GetNames(property_sub);
                                CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                                CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "addressRegion");

                                CSEM_List *values = NULL, *types = NULL;
                                CSEM_Property_GetValues(property_sub, &values, &types);
                                CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                                CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                                {/* addressRegion : FL */
                                    CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "FL");
                                }
                            }
                        }
                    }
                }{/* @property = "telephone" */
                    CSEM_Property *property_root = CSEM_List_Get(properties_root, 3);
                    CSEM_List *names = CSEM_Property_GetNames(property_root);
                    CU_ASSERT_EQUAL(CSEM_List_Size(names), 1);
                    CU_ASSERT_STRING_EQUAL(CSEM_List_Get(names, 0), "telephone");

                    CSEM_List *values = NULL, *types = NULL;
                    CSEM_Property_GetValues(property_root, &values, &types);
                    CU_ASSERT_EQUAL(CSEM_List_Size(values), 1);
                    CU_ASSERT_EQUAL(CSEM_List_Size(types), 1);
                    {/* telephone : "850-648-4200" */
                        CU_ASSERT_EQUAL(*((int *)CSEM_List_Get(types, 0)), CSEM_VALUE_TYPE_STR);
                        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(values, 0), "850-648-4200");
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
