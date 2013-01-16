/* $Id$ */
#include <string.h>

#include "csem/csem_micro_tree.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_Micro_Item_Create(CSEM_Item **item) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Item *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Item)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = CSEM_Node_Create(&(result -> node), CSEM_NODE_TYPE_MICRO_ITEM, result, NULL))) {
        goto ERROR;
    }

    *item = result;
    return error;
ERROR:
    CSEM_Micro_Item_Dispose(result);
    return error;
}
CSEM_Error CSEM_Micro_Id_Create(CSEM_Id **id, const char *value) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Id *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Id)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = CSEM_Node_Create(&(result -> node),
            CSEM_NODE_TYPE_MICRO_ID, result, NULL))) {
        goto ERROR;
    }
    result -> id = (char *)value;

    /* result */
    *id = result;
    return error;
ERROR:
    CSEM_Micro_Id_Dispose(result);
    return error;
}
CSEM_Error CSEM_Micro_Property_Create(CSEM_Property **property, CSEM_List *names) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Property *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Property)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = CSEM_Node_Create(&(result -> node), CSEM_NODE_TYPE_MICRO_PROPERTY, result, NULL))) {
        goto ERROR;
    }

    result -> names = names;

    if(!(result -> values = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if(!(result -> valueTypes = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }

    /* result */
    *property = result;

    return error;
ERROR:
    CSEM_Micro_Property_Dispose(result);
    return error;
}
char *CSEM_Micro_Item_GetId(CSEM_Item *item) {
    return item -> id;
}
CSEM_Error CSEM_Micro_Item_SetId(CSEM_Item *item, const char *id, CSEM_Bool doFree) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(!id) {
        error = CSEM_ERROR_PARAMETER;
        goto FINISH;
    }
    if(doFree) {
        CSEM_Free(item -> id);
    }
    item -> id = (char *)id;

FINISH:
    return error;
}
CSEM_List *CSEM_Micro_Item_GetTypes(const CSEM_Item *item) {
    return item -> types;
}
CSEM_Error CSEM_Micro_Item_SetTypes(CSEM_Item *item, const CSEM_List *types, CSEM_Bool doFree) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(doFree) {
        CSEM_List_Dispose(item -> types, CSEM_TRUE);
    }
    item -> types = (CSEM_List *)types;
    return error;
}
CSEM_List *CSEM_Micro_Item_GetRefs(CSEM_Item *item) {
    return item -> refs;
}
CSEM_Error CSEM_Micro_Item_SetRefs(CSEM_Item *item, const CSEM_List *refs, CSEM_Bool doFree) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(doFree) {
        CSEM_List_Dispose(item -> refs, CSEM_TRUE);
    }
    item -> refs = (CSEM_List *)refs;
    return error;
}
CSEM_List *CSEM_Micro_Item_GetProperties(const CSEM_Item *item) {
    return item -> props;
}
CSEM_Error CSEM_Micro_Item_GetNamedPropertes(const CSEM_Item *item, const char *name, CSEM_List **properties) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_List *result = NULL;

    /* init */
    if(!(result = CSEM_List_Create(8))) {
        error = CSEM_ERROR_NONE;
        goto ERROR;
    }
    {/* search */
        int i = 0;
        CSEM_List *tmpProps = CSEM_Micro_Item_GetProperties(item);
        for(i = 0; tmpProps && i < CSEM_List_Size(tmpProps); i++) {
            int j = 0;
            CSEM_Property *property = CSEM_List_Get(tmpProps, i);
            CSEM_List *tmpNames = CSEM_Micro_Property_GetNames(property);
            for(j = 0; tmpNames && j < CSEM_List_Size(tmpNames); j++) {
                char *tmpName = CSEM_List_Get(tmpNames, j);
                if(name && tmpName && !strcmp(name, tmpName)) {
                    if((error = CSEM_List_Add(result, property))) {
                        goto ERROR;
                    }
                    break;
                }
            }
        }
    }
    /* result */
    *properties = result;

    return error;
ERROR:
    CSEM_List_Dispose(result, CSEM_FALSE);
    return error;
}
CSEM_Error CSEM_Micro_Item_AddProperty(CSEM_Item *item, CSEM_Property *property, CSEM_Bool changeParent) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(!property) {
        error = CSEM_ERROR_PARAMETER;
        goto FINISH;
    }
    if(!item -> props) {
        if(!(item -> props = CSEM_List_Create(8))) {
            error = CSEM_ERROR_MEMORY;
            goto FINISH;
        }
    }
    if((error = CSEM_List_Add(item -> props, property))) {
        error = CSEM_ERROR_MEMORY;
        goto FINISH;
    }
    if(changeParent) {
        property -> node -> parent = item -> node;
    }
FINISH:
    return error;
}
CSEM_List *CSEM_Micro_Id_GetProperties(CSEM_Id *id) {
    return id -> props;
}
CSEM_Error CSEM_Micro_Id_AddProperty(CSEM_Id *id, CSEM_Property *property) {
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
char *CSEM_Micro_Id_GetId(CSEM_Id *id) {
    return id -> id;
}
CSEM_List *CSEM_Micro_Property_GetNames(CSEM_Property *property) {
    return property -> names;
}
CSEM_Error CSEM_Micro_Property_GetValues(CSEM_Property *property,
        CSEM_List **values, CSEM_List **valueTypes) {
    CSEM_Error error = CSEM_ERROR_NONE;

    if(!values || !valueTypes) {
        error = CSEM_ERROR_PARAMETER;
        goto FINISH;
    }
    /* result */
    *values = property -> values;
    *valueTypes = property -> valueTypes;

FINISH:
    return error;
}
CSEM_Error CSEM_Micro_Property_AddValues(CSEM_Property *property,
        void *value, CSEM_MICRO_VALUE_TYPE type) {
    CSEM_Error error = CSEM_ERROR_NONE;
    int *tmpType = NULL;

    if((error = CSEM_List_Add(property -> values, value))) {
        goto FINISH;
    }
    if(type == CSEM_MICRO_VALUE_TYPE_ITEM) {
        CSEM_Item *item = value;
        item -> node -> parent = property -> node;
    } else if(type == CSEM_MICRO_VALUE_TYPE_PROPERTY) {
        CSEM_Property *tmpProperty = value;
        tmpProperty -> node -> parent = property -> node;
    }

    /* set value type */
    if(!(tmpType = CSEM_Malloc(sizeof(int)))) {
        error = CSEM_ERROR_MEMORY;
        goto FINISH;
    }
    memcpy(tmpType, &type, sizeof(int));
    if((error = CSEM_List_Add(property -> valueTypes, tmpType))) {
        goto FINISH;
    }
FINISH:
    return error;
}
static CSEM_Error csem_micro_item_getItems(const CSEM_Item *item, const CSEM_List *types, CSEM_List *items) {
    CSEM_Error error = CSEM_ERROR_NONE;
    int i = 0;

    if(types) {/* check the types of the item itself */
        for(i = 0; i < CSEM_List_Size(types); i++) {
            char *type = CSEM_List_Get(types, i);
            if(!type || !strcmp(type, "")) {
                if((error = CSEM_List_Add(items, item))) {
                    goto FINISH;
                }
                goto FINISH_APPEND;
            } else {
                int j = 0;
                CSEM_List *itemTypes = CSEM_Micro_Item_GetTypes(item);
                for(j = 0; itemTypes && j < CSEM_List_Size(itemTypes); j++) {
                    char *itemType = CSEM_List_Get(itemTypes, j);
                    if(!strcmp(type, itemType)) {
                        if((error = CSEM_List_Add(items, item))) {
                            goto FINISH;
                        }
                        goto FINISH_APPEND;
                    }
                }
            }
        }
    } else {
        if((error = CSEM_List_Add(items, item))) {
            goto FINISH;
        }
    }
FINISH_APPEND:
    {/* check descendant items */
        CSEM_List *properties = CSEM_Micro_Item_GetProperties(item);
        for(i = 0; properties && i < CSEM_List_Size(properties); i++) {
            CSEM_Property *property = CSEM_List_Get(properties, i);
            CSEM_List *values = NULL, *valueTypes = NULL;
            CSEM_Micro_Property_GetValues(property, &values, &valueTypes);
            {/* check item value */
                int j = 0;
                int valueSize = CSEM_List_Size(values);
                for(j = 0; j < valueSize; j++) {
                    if(*((int *)CSEM_List_Get(valueTypes, j)) == CSEM_MICRO_VALUE_TYPE_ITEM) {
                        CSEM_Item *childItem = CSEM_List_Get(values, j);
                        if((error = csem_micro_item_getItems(childItem, types, items))) {
                            goto FINISH;
                        }
                    }
                }
            }
        }
    }
FINISH:
    return error;
}
CSEM_Error CSEM_Micro_Document_GetItems(CSEM_Document *doc, CSEM_List *types, CSEM_List **items) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_List *result = NULL;
    CSEM_List *nodeList = NULL;
    int i = 0;

    if(!items) {
        error = CSEM_ERROR_PARAMETER;
        goto ERROR;
    }

    /* init */
    if(!(result = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    nodeList = CSEM_Document_GetChildren(doc);

    for(i = 0; i < CSEM_List_Size(nodeList); i++) {
        CSEM_Node *node = CSEM_List_Get(nodeList, i);
        if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_MICRO_ITEM) {
            CSEM_Item *item = CSEM_Node_GetObject(node);
            if((error = csem_micro_item_getItems(item, types, result))) {
                goto ERROR;
            }
        }
    }
    /* result */
    *items = result;

    return error;
ERROR:
    CSEM_List_Dispose(result, CSEM_FALSE);
    return error;
}
CSEM_Error CSEM_Micro_Properties_GetValues(CSEM_List *properties, CSEM_List **values, CSEM_List **valueTypes) {
    CSEM_Error error = CSEM_ERROR_NONE;
    int i = 0;
    CSEM_List *res_values = NULL, *res_valueTypes = NULL;

    if(!values || !valueTypes) {
        error = CSEM_ERROR_PARAMETER;
        goto ERROR;
    }
    /* init */
    if(!(res_values = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if(!(res_valueTypes = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    /* collect values */
    for(i = 0; properties && i < CSEM_List_Size(properties); i++) {
        int j = 0;
        CSEM_List *tmpValues = NULL, *tmpValueTypes = NULL;
        CSEM_Property *property = CSEM_List_Get(properties, i);
        if((error = CSEM_Micro_Property_GetValues(property, &tmpValues, &tmpValueTypes))) {
            goto ERROR;
        }
        for(j = 0; tmpValues && j < CSEM_List_Size(tmpValues); j++) {
            CSEM_List_Add(res_values, CSEM_List_Get(tmpValues, j));
            CSEM_List_Add(res_valueTypes, CSEM_List_Get(tmpValueTypes, j));
        }
    }
    /* result */
    *values = res_values;
    *valueTypes = res_valueTypes;

    return error;
ERROR:
    CSEM_List_Dispose(res_values, CSEM_FALSE);
    CSEM_List_Dispose(res_valueTypes, CSEM_FALSE);
    return error;
}
void CSEM_Micro_Item_Dispose(CSEM_Item *item) {
    if(item) {
        {/* dispose properties */
            int i = 0;
            int size = item -> props ? CSEM_List_Size(item -> props) : 0;
            for(i = 0; i < size; i++) {
                CSEM_Property *property = CSEM_List_Get(item -> props, i);
                if(CSEM_Node_GetType(property -> node -> parent) != CSEM_NODE_TYPE_MICRO_ID) {
                    CSEM_Micro_Property_Dispose(property);
                }
            }
            CSEM_List_Dispose(item -> props, CSEM_FALSE);
        }
        CSEM_Node_Dispose(item -> node);
        CSEM_Free(item -> id);
        CSEM_List_Dispose(item -> types, CSEM_TRUE);
        CSEM_List_Dispose(item -> refs, CSEM_TRUE);
        CSEM_Free(item);
    }
}
void CSEM_Micro_Id_Dispose(CSEM_Id *id) {
    if(id) {
        {/* dispose properties */
            int i = 0;
            int size = id -> props ? CSEM_List_Size(id -> props) : 0;
            for(i = 0; i < size; i++) {
                CSEM_Micro_Property_Dispose(CSEM_List_Get(id -> props, i));
            }
            CSEM_List_Dispose(id -> props, CSEM_FALSE);
        }
        CSEM_Node_Dispose(id -> node);
        CSEM_Free(id -> id);
        CSEM_Free(id);
    }
}
static void csem_microdata_property_disposeValues(CSEM_Property *property) {
    if(property && property -> values) {
        int i = 0;
        int size = CSEM_List_Size(property -> values);
        for(i = 0; i < size; i++) {
            CSEM_MICRO_VALUE_TYPE *type = CSEM_List_Get(property -> valueTypes, i);
            void *value = CSEM_List_Get(property -> values, i);
            if(*type == CSEM_MICRO_VALUE_TYPE_STR) {
                CSEM_Free(value);
            } else if(*type == CSEM_MICRO_VALUE_TYPE_URL) {
                CSEM_Free(value);
            } else if(*type == CSEM_MICRO_VALUE_TYPE_ITEM) {
                CSEM_Micro_Item_Dispose(value);
            } else if(*type == CSEM_MICRO_VALUE_TYPE_PROPERTY) {
                CSEM_Micro_Property_Dispose(value);
            } else {
                fprintf(stderr, "not implemented node type [%d]\n", *type);
            }
        }
        CSEM_List_Dispose(property -> values, CSEM_FALSE);
        CSEM_List_Dispose(property -> valueTypes, CSEM_TRUE);
    }
}
void CSEM_Micro_Property_Dispose(CSEM_Property *property) {
    if(property) {
        CSEM_Node_Dispose(property -> node);
        CSEM_List_Dispose(property -> names, CSEM_TRUE);
        csem_microdata_property_disposeValues(property);
        CSEM_Free(property);
    }
}
