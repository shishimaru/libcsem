/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include <unistd.h>

#include "csem/csem_stream.h"
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"
#include "csem_utils.h"
#include "csem_types.h"

/*#define CSEM_DEBUG_BUILDER*/

extern const char *HTML5_SPACES;
static void handler_error(const void *userdata, CSEM_Error error) {
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
    if((error = CSEM_Item_Create(&item))) {
        goto ERROR;
    }
    CSEM_Item_SetTypes(item, types, CSEM_TRUE);
    CSEM_Item_SetRefs(item, refs, CSEM_TRUE);
    CSEM_Item_SetId(item, id, CSEM_TRUE);

    /* append */
    if(activeType == CSEM_NODE_TYPE_PROPERTY) {
        CSEM_Property_AddValues(activeNode -> obj.property,
                item, CSEM_VALUE_TYPE_ITEM);
    } else if(activeType == CSEM_NODE_TYPE_DOCUMENT) {
        CSEM_Document_AppendChild(builder -> document, item -> node);
    } else if(activeType == CSEM_NODE_TYPE_ITEM
            || activeType == CSEM_NODE_TYPE_ID) {/* TODO */
        CSEM_Document_AppendChild(builder -> document, item -> node);
    } else {
        fprintf(stderr, "not implemented node type [%d]\n", activeType);
        handler_error(userdata, CSEM_ERROR_PARAMETER);
    }
    /* update state */
    builder -> activeNode = item -> node;

    return CSEM_FALSE;
ERROR:
    handler_error(userdata, error);
    return CSEM_TRUE;
}
static CSEM_Bool rdfa_startScope(const void *userdata, const CSEM_List *types, const char *resource) {
    return microdata_startScope(userdata, types, NULL, resource);
}
static void microdata_endScope(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
#ifdef CSEM_DEBUG_BUILDER
    puts("@END_SCOPE");
#endif
    /* update state */
    if(builder -> activeNode -> parent) {
        builder -> activeNode = builder -> activeNode -> parent;
    }
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
    printf("@START_PROP : %s\n", propName);
#endif

    /* init */
    if((error = CSEM_Utils_Strtoks((char *)propName, HTML5_SPACES, &propNameList))) {
        goto ERROR;
    }
    if((error = CSEM_Property_Create(&property, propNameList))) {
        goto ERROR;
    }
    /* append */
    if(activeType == CSEM_NODE_TYPE_ITEM) {
        if((error = CSEM_Item_AddProperty(activeNode -> obj.item, property, CSEM_TRUE))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_ID) {
        if((error = CSEM_Id_AddProperty(activeNode -> obj.id, property))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_PROPERTY) {
        CSEM_Property *tmpProperty = activeNode -> obj.property;
        if(builder -> buf) {
            if((error = CSEM_Property_AddValues(tmpProperty, builder -> buf, builder -> propValueType))) {
                goto ERROR;
            }
        }
        if((error = CSEM_Property_AddValues(tmpProperty, property, CSEM_VALUE_TYPE_PROPERTY))) {
            goto ERROR;
        }
    } else if(activeType == CSEM_NODE_TYPE_DOCUMENT) {/* TODO */
        CSEM_Document *tmpDoc = activeNode -> obj.doc;
        if((error = CSEM_Document_AppendChild(tmpDoc, property -> node))) {
            goto ERROR;
        }
    } else {
        fprintf(stderr, "not implemented node type [%d]\n", activeType);
        handler_error(userdata, CSEM_ERROR_PARAMETER);
    }
    /* update state */
    builder -> buf = NULL;
    builder -> propValueType = hasUrlValue ?
            CSEM_VALUE_TYPE_URL : CSEM_VALUE_TYPE_STR;
    builder -> activeNode = property -> node;

    return CSEM_TRUE;
ERROR:
    CSEM_List_Dispose(propNameList, CSEM_TRUE);
    CSEM_Property_Dispose(property);
    handler_error(userdata, error);
    return CSEM_TRUE;
}
static void microdata_itemProp(const void *userdata,
        const char *value, int len) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = (void *)userdata;
    char *tmpBuf = NULL;

#ifdef CSEM_DEBUG_BUILDER
    printf("@PROP_VALUE : ");
    fwrite(value, 1, len, stdout);
    printf("\n");
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
            if(!(tmpBuf = CSEM_Realloc(builder -> buf, builder -> bufLen + len + 1))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
            builder -> buf = tmpBuf;
            builder -> bufLen += len + 1;
        }
    }
    strncat(builder -> buf + builder -> bufReadLen, value, len);
    builder -> bufReadLen += len;
    return;
ERROR:
    handler_error(userdata, error);
}
static void microdata_endProp(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
    CSEM_Node *activeNode = builder -> activeNode;

#ifdef CSEM_DEBUG_BUILDER
    puts("@END_PROP");
#endif
    /* store the concatenated property value */
    if(builder -> buf) {
        if(activeNode -> type == CSEM_NODE_TYPE_PROPERTY) {
            CSEM_Property_AddValues(activeNode -> obj.property,
                    builder -> buf, builder -> propValueType);
        } else {
            CSEM_Free(builder -> buf);
        }
    }
    /* update state */
    builder -> buf = NULL;
    if(builder -> activeNode -> parent) {
        builder -> activeNode = builder -> activeNode -> parent;
    }
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
    if((error = CSEM_Id_Create(&id, idValue))) {
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
    CSEM_Id_Dispose(id);
    handler_error(userdata, error);
    return CSEM_TRUE;
}
static void microdata_endId(const void *userdata) {
    CSEM_Builder *builder = (void *)userdata;
#ifdef CSEM_DEBUG_BUILDER
    puts("@END_ID");
#endif

    /* update state */
    if(builder -> activeNode -> parent) {
        builder -> activeNode = builder -> activeNode -> parent;
    }
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
    CSEM_List *refs = CSEM_Item_GetRefs(item);
    int i = 0;

    /* resolve this item itself */
    for(i = 0; refs && i < CSEM_List_Size(refs); i++) {
        char *ref = CSEM_List_Get(refs, i);
        int j = 0;

        for(j = 0; ref && ids && j < CSEM_List_Size(ids); j++) {
            CSEM_Node *idNode = CSEM_List_Get(ids, j);
            CSEM_Id *id = CSEM_Node_GetObject(idNode);
            char *idValue = CSEM_Id_GetId(id);

            if(idValue && !strcmp(ref, idValue)) {
                CSEM_List *props = CSEM_Id_GetProperties(id);
                int k = 0;
                int propSize = CSEM_List_Size(props);

                for(k = 0; k < propSize; k++) {
                    if((error = CSEM_Item_AddProperty(item,
                            CSEM_List_Get(props, k), CSEM_FALSE))) {
                        goto FINISH;
                    }
                }
            }
        }
    }
    {/* resolve descendant items */
        CSEM_List *props = CSEM_Item_GetProperties(item);
        for(i = 0; props && i < CSEM_List_Size(props); i++) {
            CSEM_Property *property = CSEM_List_Get(props, i);
            CSEM_List *values = NULL, *valueTypes = NULL;
            int j = 0;
            CSEM_Property_GetValues(property, &values, &valueTypes);
            for(j = 0; j < CSEM_List_Size(values); j++) {
                if(*((int *)CSEM_List_Get(valueTypes, j)) == CSEM_VALUE_TYPE_ITEM) {
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
static CSEM_Error csem_builder_resolveDocument(CSEM_Document *doc) {
    CSEM_Error error = CSEM_ERROR_NONE;
    int i = 0;
    CSEM_List *nodeList = NULL;
    CSEM_List *ids = NULL;

    {/* init */
        nodeList = CSEM_Document_GetChildren(doc);
        if((error = csem_builder_getTopNodes(doc, CSEM_NODE_TYPE_ID, &ids))) {
            goto FINISH;
        }
    }
    for(i = 0; nodeList && i < CSEM_List_Size(nodeList); i++) {
        CSEM_Node *node = CSEM_List_Get(nodeList, i);
        if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_ITEM) {
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
CSEM_Error CSEM_Builder_Parse(CSEM_Builder *builder, int fd) {
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
    return error;
ERROR:
    CSEM_Document_Dispose(builder -> document);
    return error;
}
CSEM_Error CSEM_Builder_ParseChunk(CSEM_Builder *builder,
        const char *chunk, int size, int terminate) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* init */
    if(!builder -> document) {
        if((error = CSEM_Document_Create(&(builder -> document)))) {
            goto ERROR;
        }
        builder -> activeNode = builder -> document -> node;
    }

    /* building */
    if((error = CSEM_Parser_ParseChunk(builder -> parser, chunk, size, terminate))) {
        goto ERROR;
    }
    if((error = builder -> error)) {
        goto ERROR;
    }

    return error;
ERROR:
    CSEM_Document_Dispose(builder -> document);
    return error;
}
CSEM_Error CSEM_Builder_GetDocument(CSEM_Builder *builder, CSEM_Document **doc) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* simplify the tree */
    if((error = csem_builder_resolveDocument(builder -> document))) {
        goto FINISH;
    }
    /* result */
    *doc = builder -> document;
FINISH:
    return error;
}
CSEM_Error CSEM_Builder_Create(CSEM_Builder **builder) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *result = NULL;
    CSEM_Micro_Handlers *microHandler = NULL;
    CSEM_RDFa_Handlers *rdfaHandler = NULL;

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
        if((error = CSEM_RDFa_CreateHandler(&rdfaHandler))) {
            goto ERROR;
        }
        /* handler for microdata */
        CSEM_Micro_SetItemStart(microHandler, microdata_startScope);
        CSEM_Micro_SetItemEnd(microHandler, microdata_endScope);
        CSEM_Micro_SetPropStart(microHandler, microdata_startProp);
        CSEM_Micro_SetPropValue(microHandler, microdata_itemProp);
        CSEM_Micro_SetPropEnd(microHandler, microdata_endProp);
        CSEM_Micro_SetIdStart(microHandler, microdata_startId);
        CSEM_Micro_SetIdEnd(microHandler, microdata_endId);
        CSEM_Handler_SetMicrodataHandler(result -> handler, microHandler);
        /* handler for RDFa */
        CSEM_RDFa_SetItemStart(rdfaHandler, rdfa_startScope);
        CSEM_RDFa_SetItemEnd(rdfaHandler, microdata_endScope);
        CSEM_RDFa_SetPropStart(rdfaHandler, microdata_startProp);
        CSEM_RDFa_SetPropValue(rdfaHandler, microdata_itemProp);
        CSEM_RDFa_SetPropEnd(rdfaHandler, microdata_endProp);
        CSEM_Handler_SetRDFaHandler(result -> handler, rdfaHandler);
        CSEM_Handler_SetErrorHandler(result -> handler, handler_error);

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
    CSEM_RDFa_DisposeHandler(rdfaHandler);
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
