#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "csem/csem_list.h"

void test_list() {
    int init_size;

    for(init_size = 1; init_size < 8; init_size++) {
        CSEM_List *mylist = CSEM_List_Create(init_size);
        {//add

            CU_ASSERT_EQUAL(CSEM_List_IsEmpty(mylist), CSEM_TRUE);
            CU_ASSERT_EQUAL(CSEM_List_Add(mylist, "a"), CSEM_ERROR_NONE);
            CU_ASSERT_EQUAL(CSEM_List_Add(mylist, "b"), CSEM_ERROR_NONE);

            CU_ASSERT_EQUAL(CSEM_List_Size(mylist), 2);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 0), "a");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 1), "b");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, -1), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 2), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Contains(mylist, "b"), CU_TRUE);

            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "a"), 0);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "b"), 1);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "c"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IsEmpty(mylist), CU_FALSE);
            //replace

            CU_ASSERT_EQUAL(CSEM_List_Replace(mylist, 0, "A"), "a");
            CU_ASSERT_EQUAL(CSEM_List_Replace(mylist, 1, "B"), "b");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 0), "A");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 1), "B");
            CU_ASSERT_EQUAL(CSEM_List_Replace(mylist, 0, "a"), "A");
            CU_ASSERT_EQUAL(CSEM_List_Replace(mylist, 1, "b"), "B");
        }
        {//remove
            CU_ASSERT_EQUAL(CSEM_List_Remove(mylist, 0), "a");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 0), "b");
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 1), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, -1), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 2), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Contains(mylist, "a"), CU_FALSE);
            CU_ASSERT_EQUAL(CSEM_List_Contains(mylist, "b"), CU_TRUE);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "a"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "b"), 0);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "c"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IsEmpty(mylist), CU_FALSE);
        }
        {//clear

            CSEM_List_Clear(mylist,0);
            CU_ASSERT_EQUAL(CSEM_List_Size(mylist), 0);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 0), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, 1), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Get(mylist, -1), NULL);
            CU_ASSERT_EQUAL(CSEM_List_Contains(mylist, "a"), CU_FALSE);
            CU_ASSERT_EQUAL(CSEM_List_Contains(mylist, "b"), CU_FALSE);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "a"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "b"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IndexOf(mylist, "c"), -1);
            CU_ASSERT_EQUAL(CSEM_List_IsEmpty(mylist), CU_TRUE);
        }

        CSEM_List_Dispose(mylist,0);
    }
}
void test_list_clear() {
    int *values[10];
    int i;
    for(i = 0; i < 10; i++) {
        values[i] = malloc(sizeof(int));
        memcpy(values[i], &i, sizeof(int));
        //printf("%d\n", *values[i]);
    }

    //check
    CSEM_List *list = CSEM_List_Create(8);
    for(i = 0; i < 10; i++) {
        CSEM_List_Add(list, values[i]);
    }
    CSEM_List_Clear(list, 1);
    CSEM_List_Dispose(list, 0);
}
void test_list_dispose() {
    int *values[10];
    int i;
    for(i = 0; i < 10; i++) {
        values[i] = malloc(sizeof(int));
        memcpy(values[i], &i, sizeof(int));
        //printf("%d\n", *values[i]);
    }

    //check
    CSEM_List *list = CSEM_List_Create(8);
    for(i = 0; i < 10; i++) {
        CSEM_List_Add(list, values[i]);
    }
    CSEM_List_Dispose(list, 1);
}
