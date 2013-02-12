/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>

#include "csem/csem_micro_tree.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_Id_Create(CSEM_Id **id, const char *value) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Id *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Id)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = CSEM_Node_Create(&(result -> node),
            CSEM_NODE_TYPE_ID, result, NULL))) {
        goto ERROR;
    }
    result -> id = (char *)value;

    /* result */
    *id = result;
    return error;
ERROR:
    CSEM_Id_Dispose(result);
    return error;
}
CSEM_List *CSEM_Item_GetRefs(CSEM_Item *item) {
    return item -> refs;
}
CSEM_Error CSEM_Item_SetRefs(CSEM_Item *item, const CSEM_List *refs, CSEM_Bool doFree) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(doFree) {
        CSEM_List_Dispose(item -> refs, CSEM_TRUE);
    }
    item -> refs = (CSEM_List *)refs;
    return error;
}
CSEM_List *CSEM_Id_GetProperties(CSEM_Id *id) {
    return id -> props;
}
CSEM_Error CSEM_Id_AddProperty(CSEM_Id *id, CSEM_Property *property) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(!property) {
        error = CSEM_ERROR_PARAMETER;
        goto FINISH;
    }
    if(!id -> props) {
        if(!(id -> props = CSEM_List_Create(8))) {
            error = CSEM_ERROR_MEMORY;
            goto FINISH;
        }
    }
    if((error = CSEM_List_Add(id -> props, property))) {
        error = CSEM_ERROR_MEMORY;
        goto FINISH;
    }
    property -> node -> parent = id -> node;
FINISH:
    return error;
}
char *CSEM_Id_GetId(CSEM_Id *id) {
    return id -> id;
}
void CSEM_Id_Dispose(CSEM_Id *id) {
    if(id) {
        {/* dispose properties */
            int i = 0;
            int size = id -> props ? CSEM_List_Size(id -> props) : 0;
            for(i = 0; i < size; i++) {
                CSEM_Property_Dispose(CSEM_List_Get(id -> props, i));
            }
            CSEM_List_Dispose(id -> props, CSEM_FALSE);
        }
        CSEM_Node_Dispose(id -> node);
        CSEM_Free(id -> id);
        CSEM_Free(id);
    }
}
