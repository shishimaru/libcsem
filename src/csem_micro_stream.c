/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_Micro_CreateHandler(CSEM_Micro_Handlers **handler) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Micro_Handlers *result = NULL;
    int *initScopeDepth = NULL;
    int *initIdDepth = NULL;
    int *initPropDepth = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Micro_Handlers)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    result -> currentDepth = -1;
    {/* add -1 to scopeDepth */
        int value = -1;
        result -> itemDepth = CSEM_Stack_Create(8);
        if(!(initScopeDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initScopeDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> itemDepth, initScopeDepth);
    }
    {/* add -1 to idDepth */
        /*result -> idDepth = -1; TODO */
        int value = -1;
        result -> idDepth = CSEM_Stack_Create(8);
        if(!(initIdDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initIdDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> idDepth, initIdDepth);
    }
    {/* add -1 to propDepth */
        int value = -1;
        result -> propDepth = CSEM_Stack_Create(8);
        if(!(initPropDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initPropDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> propDepth, initPropDepth);
    }

    /* result */
    *handler = result;
    return error;
ERROR:
    CSEM_Micro_DisposeHandler(result);
    CSEM_Free(initScopeDepth);
    CSEM_Free(initPropDepth);
    return error;
}
void CSEM_Micro_DisposeHandler(CSEM_Micro_Handlers *handler) {
    if(handler) {
        CSEM_Stack_Dispose(handler -> itemDepth, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> idDepth, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> propDepth, CSEM_TRUE);
        CSEM_Free(handler);
    }
}
void CSEM_Micro_SetItemStart(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemStart itemStart) {
    handler -> itemStart = itemStart;
}
void CSEM_Micro_SetItemEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemEnd itemEnd) {
    handler -> itemEnd = itemEnd;
}
void CSEM_Micro_SetPropStart(CSEM_Micro_Handlers *handler, CSEM_Micro_PropStart propStart) {
    handler -> propStart = propStart;
}
void CSEM_Micro_SetPropValue(CSEM_Micro_Handlers *handler, CSEM_Micro_PropValue propValue) {
    handler -> propValue = propValue;
}
void CSEM_Micro_SetPropEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_PropEnd propEnd) {
    handler -> propEnd = propEnd;
}
void CSEM_Micro_SetIdStart(CSEM_Micro_Handlers *handler, CSEM_Micro_IdStart idStart) {
    handler -> idStart = idStart;
}
void CSEM_Micro_SetIdEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_IdEnd idEnd) {
    handler -> idEnd = idEnd;
}
