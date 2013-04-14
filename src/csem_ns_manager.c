/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include "csem/csem_common.h"
#include "csem_ns_manager.h"
#include "csem_utils.h"

typedef CSEM_List CSEM_NS_List ;

struct CSEM_NSManager {
    CSEM_List *stack;
    CSEM_NS_List *top;
};
typedef struct CSEM_NS_Pairs {
    const char *prefix;
    const char *uri;
} CSEM_NS_Pairs;

static CSEM_NS_Pairs *csem_ns_pairs_create(const char *prefix, const char *uri) {
    CSEM_NS_Pairs *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_NS_Pairs)))) {
        goto FINISH;
    }
    result -> prefix = prefix;
    result -> uri = uri;
FINISH:
    return result;
}
static void csem_ns_pairs_dispose(CSEM_NS_Pairs *pairs, CSEM_Bool freeData) {
    if(pairs) {
        if(freeData) {
            CSEM_Free((char *)pairs -> prefix);
            CSEM_Free((char *)pairs -> uri);
        }
        CSEM_Free(pairs);
    }
}
static void csem_ns_list_dispose(CSEM_NS_List *nsList, CSEM_Bool freeData) {
    if(nsList) {
        int i = 0;
        FOR(i, 0, CSEM_List_Size(nsList)) {
            CSEM_NS_Pairs *pairs = CSEM_List_Get(nsList, i);
            csem_ns_pairs_dispose(pairs, freeData);
        }
        CSEM_List_Dispose(nsList, CSEM_FALSE);
    }
}
CSEM_NSManager *CSEM_NSManager_Create() {
    CSEM_NSManager *result = NULL;
    if(!(result = CSEM_Calloc(1, sizeof(CSEM_NSManager)))) {
        goto FINISH;
    }
    result -> stack = CSEM_List_Create(8);
FINISH:
    return result;
}
void CSEM_NSManager_Dispose(CSEM_NSManager *manager, CSEM_Bool freeData) {
    if(manager) {
        int i = 0;
        FOR(i, 0, CSEM_List_Size(manager -> stack)) {
            CSEM_NS_List *nsList = CSEM_List_Get(manager -> stack, i);
            csem_ns_list_dispose(nsList, freeData);
        }
        CSEM_List_Dispose(manager -> stack, CSEM_FALSE);
        CSEM_Free(manager);
    }
}
CSEM_Error CSEM_NSManager_Add(CSEM_NSManager *manager,
        const char *prefix, const char *uri) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_NS_Pairs *pairs = NULL;

    if(!(pairs = csem_ns_pairs_create(prefix, uri))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if(!manager -> top) {
        if(!(manager -> top = CSEM_List_Create(4))) {
            error = CSEM_ERROR_MEMORY;
            goto ERROR;
        }
    }
    if((error = CSEM_List_Add(manager -> top, pairs))) {
        goto ERROR;
    }
    return error;
ERROR:
    csem_ns_pairs_dispose(pairs, CSEM_FALSE);
    return error;
}
CSEM_Error CSEM_NSManager_Push(CSEM_NSManager *manager) {
    CSEM_Error error = CSEM_ERROR_NONE;

    if((error = CSEM_List_Add(manager -> stack, manager -> top))) {
        goto FINISH;
    }
    manager -> top = NULL;
FINISH:
    return error;
}
static const char *csem_ns_list_getURI(CSEM_NS_List *nsList, const char *prefix) {
    const char *result = NULL;
    int i = 0;
    FOR(i, 0, CSEM_List_Size(nsList)) {
        CSEM_NS_Pairs *pairs = CSEM_List_Get(nsList, i);
        if(prefix) {
            if(pairs -> prefix && !strcmp(prefix, pairs -> prefix)) {
                result = pairs -> uri;
                break;
            }
        } else {
            if(!pairs -> prefix) {
                result = pairs -> uri;
                break;
            }
        }
    }
    return result;
}
const char *CSEM_NSManager_LookupURI(CSEM_NSManager *manager, const char *prefix) {
    const char *result = NULL;
    int i = CSEM_List_Size(manager -> stack) - 1;
    CSEM_NS_List *nsList = NULL;

    while((nsList = CSEM_List_Get(manager -> stack, i--))) {
        if((result = csem_ns_list_getURI(nsList, prefix))) {
            break;
        }
    }
    return result;
}
void CSEM_NSManager_Pop(CSEM_NSManager *manager, CSEM_Bool freeData) {
    int index = CSEM_List_Size(manager -> stack) - 1;
    CSEM_NS_List *nsList = CSEM_List_Remove(manager -> stack, index);
    csem_ns_list_dispose(nsList, freeData);
}
