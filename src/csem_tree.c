/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include <unistd.h>

#include "csem/csem_tree.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_Node_Create(CSEM_Node **node, CSEM_NODE_TYPE type,
        void *obj, CSEM_Node *parent) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Node *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Node)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    result -> parent = parent;
    result -> type = type;
    result -> obj.doc = obj;

    /* result */
    *node = result;

    return error;
ERROR:
    CSEM_Node_Dispose(result);
    return error;
}
CSEM_NODE_TYPE CSEM_Node_GetType(CSEM_Node *node) {
    return node ? node -> type : CSEM_NODE_TYPE_NONE;
}
CSEM_Node *CSEM_Node_GetParent(CSEM_Node *node) {
    return node -> parent;
}
void *CSEM_Node_GetObject(CSEM_Node *node) {
    if(node -> type == CSEM_NODE_TYPE_DOCUMENT) {
        return node -> obj.doc;
    } else if(node -> type == CSEM_NODE_TYPE_MICRO_ITEM) {
        return node -> obj.item;
    } else if(node -> type == CSEM_NODE_TYPE_MICRO_ID) {
        return node -> obj.id;
    } else if(node -> type == CSEM_NODE_TYPE_MICRO_PROPERTY) {
        return node -> obj.property;
    }
    return NULL;
}
void CSEM_Node_Dispose(CSEM_Node *node) {
    if(node) {
        CSEM_Free(node);
    }
}
CSEM_Error CSEM_Document_Create(CSEM_Document **document) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Document *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Document)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = CSEM_Node_Create(&(result -> node), CSEM_NODE_TYPE_DOCUMENT,
            result, NULL))) {
        goto ERROR;
    }
    if(!(result -> children = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }

    /* result */
    *document = result;

    return error;
ERROR:
    CSEM_Document_Dispose(result);
    return error;
}
void CSEM_Document_Dispose(CSEM_Document *document) {
    if(document) {
        int i = 0;
        int size = CSEM_List_Size(document -> children);
        for(i = 0; i < size; i++) {
            CSEM_Node *child = CSEM_List_Get(document -> children, i);
            if(child -> type == CSEM_NODE_TYPE_MICRO_ITEM) {
                CSEM_Micro_Item_Dispose(child -> obj.item);
            } else if(child -> type == CSEM_NODE_TYPE_MICRO_ID) {
                CSEM_Micro_Id_Dispose(child -> obj.id);
            } else if(child -> type == CSEM_NODE_TYPE_MICRO_PROPERTY) {
                CSEM_Micro_Property_Dispose(child -> obj.property);
            }
        }
        CSEM_List_Dispose(document -> children, CSEM_FALSE);
        CSEM_Node_Dispose(document -> node);
        CSEM_Free(document);
    }
}
CSEM_List *CSEM_Document_GetChildren(CSEM_Document *doc) {
    return doc -> children;
}
CSEM_Error CSEM_Document_AppendChild(CSEM_Document *doc, CSEM_Node *child) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(!child) {
        error = CSEM_ERROR_PARAMETER;
        goto FINISH;
    }
    if((error = CSEM_List_Add(doc -> children, child))) {
        goto FINISH;
    }
    child -> parent = doc -> node;
FINISH:
    return error;
}
