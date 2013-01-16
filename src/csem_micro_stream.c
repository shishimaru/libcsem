/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include "csem_micro_stream_internal.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_Micro_CreateHandler(CSEM_Micro_Handlers **handler) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Micro_Handlers *result = NULL;
    int *initScopeDepth = NULL;
    int *initPropDepth = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Micro_Handlers)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    result -> currentDepth = -1;
    {/* add -1 to scopeDepth */
        int value = -1;
        result -> scopeDepth = CSEM_List_Create(8);
        if(!(initScopeDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initScopeDepth, &value, sizeof(int));
        CSEM_List_Add(result -> scopeDepth, initScopeDepth);
    }
    {/* add -1 to scopeDepth */
        int value = -1;
        result -> propDepth = CSEM_List_Create(8);
        if(!(initPropDepth = CSEM_Malloc(sizeof(int)))) {
            goto ERROR;
        }
        memcpy(initPropDepth, &value, sizeof(int));
        CSEM_List_Add(result -> scopeDepth, initPropDepth);
    }
    result -> idDepth = -1;

    /* result */
    *handler = result;
    return error;
ERROR:
    CSEM_Micro_DisposeHandler(result);
    CSEM_Free(initScopeDepth);
    CSEM_Free(initPropDepth);
    return error;
}
void CSEM_Micro_SetStartScope(CSEM_Micro_Handlers *handler, CSEM_Micro_StartScope startScope) {
    handler -> startScope = startScope;
}
void CSEM_Micro_SetEndScope(CSEM_Micro_Handlers *handler, CSEM_Micro_EndScope endScope) {
    handler -> endScope = endScope;
}
void CSEM_Micro_SetStartItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_StartProp startProp) {
    handler -> startProp = startProp;
}
void CSEM_Micro_SetItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemProp itemProp) {
    handler -> itemProp = itemProp;
}
void CSEM_Micro_SetEndItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_EndProp endProp) {
    handler -> endProp = endProp;
}
void CSEM_Micro_SetStartId(CSEM_Micro_Handlers *handler, CSEM_Micro_StartId startId) {
    handler -> startId = startId;
}
void CSEM_Micro_SetEndId(CSEM_Micro_Handlers *handler, CSEM_Micro_EndId endId) {
    handler -> endId = endId;
}
void CSEM_Micro_DisposeHandler(CSEM_Micro_Handlers *handler) {
    CSEM_List_Dispose(handler -> scopeDepth, CSEM_TRUE);
    CSEM_List_Dispose(handler -> propDepth, CSEM_TRUE);
    CSEM_Free(handler);
}
char *CSEM_Micro_GetAttNameWithPropValue(const char *localname, const char *ns) {
    if(localname && (!ns || !strcmp(ns, ""))) {
        if(!strcmp(localname, "meta")) {
            return "content";
        } else if(!strcmp(localname, "audio")
                || !strcmp(localname, "embed")
                || !strcmp(localname, "iframe")
                || !strcmp(localname, "img")
                || !strcmp(localname, "source")
                || !strcmp(localname, "track")
                || !strcmp(localname, "video")) {
            return "src";
        } else if(!strcmp(localname, "a")
                || !strcmp(localname, "area")
                || !strcmp(localname, "link")) {
            return "href";
        } else if(!strcmp(localname, "object")) {
            return "data";
        } else if(!strcmp(localname, "data")) {
            return "value";
        } else if(!strcmp(localname, "time")) {
            return "datetime";
        }
    }
    return NULL;
}
CSEM_Bool CSEM_Micro_IsUrlPropElement(const char *localname, const char *ns) {
    if(localname && (!ns || !strcmp(ns, ""))) {
        if(!strcmp(localname, "audio")
                || !strcmp(localname, "embed")
                || !strcmp(localname, "iframe")
                || !strcmp(localname, "img")
                || !strcmp(localname, "source")
                || !strcmp(localname, "track")
                || !strcmp(localname, "video")
                || !strcmp(localname, "a")
                || !strcmp(localname, "area")
                || !strcmp(localname, "link")
                || !strcmp(localname, "object")) {
            return CSEM_TRUE;
        }
    }
    return CSEM_FALSE;
}
