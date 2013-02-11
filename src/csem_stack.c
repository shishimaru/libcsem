/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include "csem_utils.h"
#include "csem/csem_stack.h"
#include "csem/csem_list.h"

struct CSEM_Stack {
    CSEM_List *list;
};
CSEM_Stack *CSEM_Stack_Create(size_t initial_size) {
    CSEM_Stack *result = NULL;
    if(!(result = CSEM_Malloc(sizeof(CSEM_Stack)))) {
        goto FINISH;
    }
    result -> list = CSEM_List_Create(initial_size);
FINISH:
    return result;
}
void CSEM_Stack_Dispose(CSEM_Stack *stack, CSEM_Bool freeData) {
    if(stack) {
        CSEM_List_Dispose(stack -> list, freeData);
        CSEM_Free(stack);
    }
}
CSEM_Error CSEM_Stack_Push(CSEM_Stack *stack, const void *data) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if((error = CSEM_List_Add(stack -> list, data))) {
        goto FINISH;
    }
FINISH:
    return error;
}
void *CSEM_Stack_Pop(CSEM_Stack *stack) {
    size_t size = CSEM_List_Size(stack -> list);
    return size ? CSEM_List_Remove(stack -> list, size - 1) : NULL;
}
void *CSEM_Stack_Top(const CSEM_Stack *stack) {
    size_t size = CSEM_List_Size(stack -> list);
    return size ? CSEM_List_Get(stack -> list, size - 1) : NULL;
}
size_t CSEM_Stack_Size(const CSEM_Stack *stack) {
    return CSEM_List_Size(stack -> list);
}

