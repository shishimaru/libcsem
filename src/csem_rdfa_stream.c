/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include "csem/csem_rdfa_stream.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_RDFa_CreateHandler(CSEM_RDFa_Handlers **handler) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_RDFa_Handlers *result = NULL;
    int *initScopeDepth = NULL;
    int *initVocabDepth = NULL;
    int *initPrefixDepth = NULL;
    int *initPropDepth = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_RDFa_Handlers)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    {/* init nsManager */
        result -> nsManager = CSEM_NSManager_Create();
    }

    {/* add -1 to scopeDepth */
        int value = -1;
        result -> itemDepth = CSEM_Stack_Create(8);
        if(!(initScopeDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initScopeDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> itemDepth, initScopeDepth);
    }
    {/* add -1 to vocabDepth */
        int value = -1;
        result -> vocabDepth = CSEM_Stack_Create(8);
        if(!(initVocabDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initVocabDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> vocabDepth, initVocabDepth);
    }
    {/* add -1 to prefixDepth */
        int value = -1;
        result -> prefixDepth = CSEM_Stack_Create(8);
        if(!(initPrefixDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initPrefixDepth, &value, sizeof(int));
        CSEM_Stack_Push(result -> prefixDepth, initPrefixDepth);
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
    CSEM_RDFa_DisposeHandler(result);
    CSEM_Free(initScopeDepth);
    CSEM_Free(initPropDepth);
    return error;
}
void CSEM_RDFa_DisposeHandler(CSEM_RDFa_Handlers *handler) {
    if(handler) {
        CSEM_NSManager_Dispose(handler -> nsManager, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> itemDepth, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> vocabDepth, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> prefixDepth, CSEM_TRUE);
        CSEM_Stack_Dispose(handler -> propDepth, CSEM_TRUE);
        CSEM_Free(handler);
    }
}
void CSEM_RDFa_SetItemStart(CSEM_RDFa_Handlers *handler, CSEM_RDFa_ItemStart itemStart) {
    handler -> itemStart = itemStart;
}
void CSEM_RDFa_SetItemEnd(CSEM_RDFa_Handlers *handler, CSEM_RDFa_ItemEnd itemEnd) {
    handler -> itemEnd = itemEnd;
}
void CSEM_RDFa_SetPropStart(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropStart propStart) {
    handler -> propStart = propStart;
}
void CSEM_RDFa_SetPropValue(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropValue propValue) {
    handler -> propValue = propValue;
}
void CSEM_RDFa_SetPropEnd(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropEnd propEnd) {
    handler -> propEnd = propEnd;
}
