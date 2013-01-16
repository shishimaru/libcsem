/* $Id$ */
#include <string.h>
#include <unistd.h>

#include "csem/csem_stream.h"
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"
#include "csem_utils.h"
#include "csem_types.h"
#include "csem_micro_stream_internal.h"

static void microdata_error(const void *userdata, CSEM_Error error) {
    CSEM_Builder *builder = (void *)userdata;
#ifdef CSEM_DEBUG_BUILDER
    puts("@ERROR");
#endif
    builder -> error = error;
}
static CSEM_Bool microdata_startScope(const void *userdata,
        const CSEM_List *types, const CSEM_List *refs, const char *id) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = (void *)userdata;
    CSEM_Node *activeNode = builder -> activeNode;
    CSEM_NODE_TYPE activeType = CSEM_Node_GetType(activeNode);
    CSEM_Item *item = NULL;

#ifdef CSEM_DEBUG_BUILDER
    puts("@START_SCOPE");
#endif

    /* create new item */
    if((error = CSEM_Micro_Item_Create(&item))) {
        goto ERROR;
    }
    CSEM_Micro_Item_SetTypes(item, types, CSEM_TRUE);
    CSEM_Micro_Item_SetRefs(item, refs, CSEM_TRUE);
    CSEM_Micro_Item_SetId(item, id, CSEM_TRUE);

    /* append */
    if(activeType == CSEM_NODE_TYPE_MICRO_PROPERTY) {
        CSEM_Micro_Property_AddValues(activeNode -> obj.property,
                item, CSEM_MICRO_VALUE_TYPE_ITEM);
    } else if(activeType == CSEM_NODE_TYPE_DOCUMENT) {
        CSEM_Document_AppendChild(builder -> document, item -> node);
    } else if(activeType == CSEM_NODE_TYPE_MICRO_ITEM
            || activeType == CSEM_NODE_TYPE_MICRO_ID) {/* TODO */
        CSEM_Document_AppendChild(builder -> document, item -> node);
    } else {
        fprintf(stderr, "not implemented node type [%d]\n", activeType);
        microdata_error(userdata, CSEM_ERROR_PARAMETER);
    }
    /* update state */
    builder -> activeNode = item -> node;

    return CSEM_FALSE;
ERROR:
    microdata_error(userdata, error);
    return CSEM_TRUE;
}
static void microdata_endScope(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
#ifdef CSEM_DEBUG_BUILDER
    puts("@END_SCOPE");
#endif
    /* update state */
    builder -> activeNode = builder -> activeNode -> parent;
}
static CSEM_Bool microdata_startProp(const void *userdata,
        const char *propName, CSEM_Bool hasUrlValue) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = (void *)userdata;
    CSEM_Node *activeNode = builder -> activeNode;
    CSEM_NODE_TYPE activeType = builder -> activeNode -> type;
    CSEM_Property *property = NULL;
    CSEM_List *propNameList = NULL;

#ifdef CSEM_DEBUG_BUILDER
    puts("@START_PROP");
#endif

    /* init */
    if((error = CSEM_Utils_Strtoks((char *)propName, HTML5_SPACES, &propNameList))) {
        goto ERROR;
    }
    if((error = CSEM_Micro_Property_Create(&property, propNameList))) {
        goto ERROR;
    }
    /* append */
    if(activeType == CSEM_NODE_TYPE_MICRO_ITEM) {
        if((error = CSEM_Micro_Item_AddProperty(activeNode -> obj.item, property, CSEM_TRUE))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_MICRO_ID) {
        if((error = CSEM_Micro_Id_AddProperty(activeNode -> obj.id, property))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_MICRO_PROPERTY) {/* TODO */
        CSEM_Property *tmpProperty = activeNode -> obj.property;
        if((error = CSEM_Micro_Property_AddValues(tmpProperty, builder -> buf, CSEM_MICRO_VALUE_TYPE_STR))) {
            goto ERROR;
        }
        if((error = CSEM_Micro_Property_AddValues(tmpProperty, property, CSEM_MICRO_VALUE_TYPE_PROPERTY))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_DOCUMENT) {/* TODO */
        CSEM_Document *tmpDoc = activeNode -> obj.doc;
        if((error = CSEM_Document_AppendChild(tmpDoc, property -> node))) {
            goto ERROR;
        }
    } else {
        fprintf(stderr, "not implemented node type [%d]\n", activeType);
        microdata_error(userdata, CSEM_ERROR_PARAMETER);
    }
    /* update state */
    builder -> buf = NULL;
    builder -> propValueType = hasUrlValue ?
            CSEM_MICRO_VALUE_TYPE_URL : CSEM_MICRO_VALUE_TYPE_STR;
    builder -> activeNode = property -> node;


    return CSEM_TRUE;
ERROR:
    CSEM_List_Dispose(propNameList, CSEM_TRUE);
    CSEM_Micro_Property_Dispose(property);
    microdata_error(userdata, error);
    return CSEM_TRUE;
}
static void microdata_itemProp(const void *userdata,
        const char *value, int len) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = (void *)userdata;
    char *tmpBuf = NULL;

#ifdef CSEM_DEBUG_BUILDER
    puts("@PROP_VALUE");
#endif

    if(!builder -> buf) {
        if(!(builder -> buf = CSEM_Calloc(len + 1, sizeof(char)))) {
            error = CSEM_ERROR_MEMORY;
            goto ERROR;
        }
        builder -> bufLen = len + 1;
        builder -> bufReadLen = 0;
    } else {
        if(builder -> bufReadLen + len > builder -> bufLen - 1) {
            if(!(tmpBuf = CSEM_Realloc(builder -> buf, builder -> bufLen + len))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
            builder -> buf = tmpBuf;
            builder -> bufLen += len;
        }
    }
    strncat(builder -> buf, value, len);
    builder -> bufReadLen += len;
    return;
ERROR:
    microdata_error(userdata, error);
}
static void microdata_endProp(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
    CSEM_Node *activeNode = builder -> activeNode;

#ifdef CSEM_DEBUG_BUILDER
    puts("@END_PROP");
#endif
    /* store the concatenated property value */
    if(builder -> buf) {
        CSEM_Micro_Property_AddValues(activeNode -> obj.property,
                builder -> buf, builder -> propValueType);
    }
    /* update state */
    builder -> buf = NULL;
    builder -> activeNode = builder -> activeNode -> parent;
}
static CSEM_Bool microdata_startId(const void *userdata,
        const char *idValue) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = (void *)userdata;
    CSEM_Id *id = NULL;

#ifdef CSEM_DEBUG_BUILDER
    puts("@START_ID");
#endif

    /* init */
    if((error = CSEM_Micro_Id_Create(&id, idValue))) {
        goto ERROR;
    }
    /* append to document */
    if((error = CSEM_Document_AppendChild(builder -> document, id -> node))) {
        goto ERROR;
    }
    /* update state */
    builder -> activeNode = id -> node;

    return CSEM_FALSE;
ERROR:
    CSEM_Micro_Id_Dispose(id);
    microdata_error(userdata, error);
    return CSEM_TRUE;
}
static void microdata_endId(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
#ifdef CSEM_DEBUG_BUILDER
    puts("@END_ID");
#endif

    /* update state */
    builder -> activeNode = builder -> activeNode -> parent;
}
CSEM_Error csem_builder_getTopNodes(CSEM_Document *doc, CSEM_NODE_TYPE type, CSEM_List **nodes) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_List *result = NULL;
    CSEM_List *nodeList = NULL;
    int i = 0, size = 0;

    {/* init */
        if(!(result = CSEM_List_Create(8))) {
            error = CSEM_ERROR_MEMORY;
            goto ERROR;
        }
        nodeList = CSEM_Document_GetChildren(doc);
        size = CSEM_List_Size(nodeList);
    }
    /* filter */
    for(i = 0; i < size; i++) {
        CSEM_Node *node = CSEM_List_Get(nodeList, i);
        if(CSEM_Node_GetType(node) == type) {
            CSEM_List_Add(result, node);
        }
    }
    /* result */
    *nodes = result;
    return error;
ERROR:
    CSEM_List_Dispose(result, CSEM_TRUE);
    return error;
}
CSEM_Error csem_builder_resolveItem(CSEM_Item *item, CSEM_List *ids) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
    int i = 0;

    /* resolve this item itself */
    for(i = 0; refs && i < CSEM_List_Size(refs); i++) {
        char *ref = CSEM_List_Get(refs, i);
        int j = 0;

        for(j = 0; ref && ids && j < CSEM_List_Size(ids); j++) {
            CSEM_Node *idNode = CSEM_List_Get(ids, j);
            CSEM_Id *id = CSEM_Node_GetObject(idNode);
            char *idValue = CSEM_Micro_Id_GetId(id);

            if(idValue && !strcmp(ref, idValue)) {
                CSEM_List *props = CSEM_Micro_Id_GetProperties(id);
                int k = 0;
                int propSize = CSEM_List_Size(props);

                for(k = 0; k < propSize; k++) {
                    if((error = CSEM_Micro_Item_AddProperty(item, CSEM_List_Get(props, k), CSEM_FALSE))) {
                        goto FINISH;
                    }
                }
            }
        }
    }
    {/* resolve descendant items */
        CSEM_List *props = CSEM_Micro_Item_GetProperties(item);
        for(i = 0; props && i < CSEM_List_Size(props); i++) {
            CSEM_Property *property = CSEM_List_Get(props, i);
            CSEM_List *values = NULL, *valueTypes = NULL;
            int j = 0;
            CSEM_Micro_Property_GetValues(property, &values, &valueTypes);
            for(j = 0; j < CSEM_List_Size(values); j++) {
                if(*((int *)CSEM_List_Get(valueTypes, j)) == CSEM_MICRO_VALUE_TYPE_ITEM) {
                    if((error = csem_builder_resolveItem(CSEM_List_Get(values, j), ids))) {
                        goto FINISH;
                    }
                }
            }
        }
    }

FINISH:
    return error;
}
CSEM_Error csem_builder_resolveDocument(CSEM_Document *doc) {
    CSEM_Error error = CSEM_ERROR_NONE;
    int i = 0, size = 0;
    CSEM_List *nodeList = NULL;
    CSEM_List *ids = NULL;

    {/* init */
        nodeList = CSEM_Document_GetChildren(doc);
        if((error = csem_builder_getTopNodes(doc, CSEM_NODE_TYPE_MICRO_ID, &ids))) {
            goto FINISH;
        }
        size = CSEM_List_Size(nodeList);
    }
    for(i = 0; i < size; i++) {
        CSEM_Node *node = CSEM_List_Get(nodeList, i);
        if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_MICRO_ITEM) {
            CSEM_Item *item = CSEM_Node_GetObject(node);
            if((error = csem_builder_resolveItem(item, ids))) {
                goto FINISH;
            }
        }
    }
FINISH:
    CSEM_List_Dispose(ids, CSEM_FALSE);
    return error;
}
CSEM_Error CSEM_Builder_Parse(CSEM_Builder *builder,
        int fd, CSEM_Bool resolve, CSEM_Document **doc) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* init */
    if((error = CSEM_Document_Create(&(builder -> document)))) {
        goto ERROR;
    }
    builder -> activeNode = builder -> document -> node;

    /* start building */
    if((error = CSEM_Parser_Parse(builder -> parser, fd))) {
        goto ERROR;
    }
    if((error = builder -> error)) {
        goto ERROR;
    }
    /* simplify the tree */
    if(resolve) {
        if((error = csem_builder_resolveDocument(builder -> document))) {
            goto ERROR;
        }
    }
    /* result */
    *doc = builder -> document;
    return error;
ERROR:
    CSEM_Document_Dispose(builder -> document);
    return error;
}
CSEM_Error CSEM_Builder_Create(CSEM_Builder **builder) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *result = NULL;
    CSEM_Micro_Handlers *microHandler = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Builder)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    {/* init parser */
        if((error = CSEM_Handler_Create(&(result -> handler)))) {
            goto ERROR;
        }
        if((error = CSEM_Micro_CreateHandler(&microHandler))) {
            goto ERROR;
        }
        CSEM_Micro_SetStartScope(microHandler, microdata_startScope);
        CSEM_Micro_SetEndScope(microHandler, microdata_endScope);
        CSEM_Micro_SetStartItemProp(microHandler, microdata_startProp);
        CSEM_Micro_SetItemProp(microHandler, microdata_itemProp);
        CSEM_Micro_SetEndItemProp(microHandler, microdata_endProp);
        CSEM_Micro_SetStartId(microHandler, microdata_startId);
        CSEM_Micro_SetEndId(microHandler, microdata_endId);
        CSEM_Handler_SetMicrodataHandler(result -> handler, microHandler);
        CSEM_Handler_SetErrorHandler(result -> handler, microdata_error);

        if((error = CSEM_Parser_Create(&(result -> parser)))) {
            goto ERROR;
        }
        CSEM_Parser_SetHandler(result -> parser, result -> handler);
        CSEM_Parser_SetUserdata(result -> parser, result);
    }

    /* result */
    *builder = result;
    return error;
ERROR:
    CSEM_Micro_DisposeHandler(microHandler);
    CSEM_Handler_Dispose(result -> handler, CSEM_FALSE);
    CSEM_Builder_Dispose(result);
    return error;
}
void CSEM_Builder_Dispose(CSEM_Builder *builder) {
    if(builder) {
        CSEM_Handler_Dispose(builder -> handler, CSEM_TRUE);
        CSEM_Parser_Dispose(builder -> parser);
        CSEM_Free(builder);
    }
}
