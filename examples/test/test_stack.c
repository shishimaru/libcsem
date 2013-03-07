#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "csem_stack.h"

void test_stack() {
    CSEM_Stack *stack = CSEM_Stack_Create(2);

    CU_ASSERT_EQUAL(CSEM_Stack_Size(stack), 0);
    CU_ASSERT_EQUAL(CSEM_Stack_Top(stack), NULL);
    CU_ASSERT_EQUAL(CSEM_Stack_Pop(stack), NULL);

    CSEM_Stack_Push(stack, "a");
    CSEM_Stack_Push(stack, "b");
    CSEM_Stack_Push(stack, "c");
    CSEM_Stack_Push(stack, "d");

    CU_ASSERT_EQUAL(CSEM_Stack_Size(stack), 4);
    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Top(stack), "d");
    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Pop(stack), "d");

    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Top(stack), "c");
    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Pop(stack), "c");

    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Top(stack), "b");
    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Pop(stack), "b");

    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Top(stack), "a");
    CU_ASSERT_STRING_EQUAL(CSEM_Stack_Pop(stack), "a");

    CSEM_Stack_Dispose(stack, CSEM_FALSE);
}
void test_stack_dispose() {
    int *values[10];
    int i;
    for(i = 0; i < 10; i++) {
        values[i] = malloc(sizeof(int));
        memcpy(values[i], &i, sizeof(int));
    }

    //check
    CSEM_Stack *stack = CSEM_Stack_Create(2);
    for(i = 0; i < 10; i++) {
        CSEM_Stack_Push(stack, values[i]);
    }
    CSEM_Stack_Dispose(stack, CSEM_TRUE);
}
