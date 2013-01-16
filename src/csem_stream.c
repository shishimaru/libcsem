/* $Id$ */
#include <string.h>
#include <unistd.h>

#include "csem_utils.h"
#include "csem_types.h"
#include "csem_micro_stream_internal.h"

const char *HTML5_SPACES = "\x20\x09\x0a\x0c\x0d";

static void sax_startElementNs(void *ctx,
        const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI,
        int nb_namespaces, const xmlChar **namespaces,
        int nb_attributes, int nb_defaulted, const xmlChar **attributes) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Parser *parser = ctx;
    int i = 0;
    CSEM_Bool isStartScope = CSEM_FALSE;
    CSEM_Bool isStartId = CSEM_FALSE;
    CSEM_List *itemtypes = NULL;
    CSEM_List *itemrefs = NULL;
    char *propName = NULL;
    char *propAttName = NULL;
    char *propValue = NULL;
    size_t propValueLen = 0;
    char *itemid = NULL;
    char *id = NULL;
    CSEM_Bool freeScope = CSEM_TRUE;
    CSEM_Bool freeId = CSEM_TRUE;
    CSEM_Bool freeProp = CSEM_TRUE;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;

    /* init state */
    if(microHandler) {
        (microHandler -> currentDepth)++;
        microHandler -> startPropValue = CSEM_FALSE;
        propAttName = CSEM_Micro_GetAttNameWithPropValue((const char *)localname, (const char *)URI);
    }

#ifdef CSEM_DEBUG_PARSER
    printf("<%s>%d\n", localname, microHandler -> currentDepth);
#endif

    for(i = 0; i < nb_attributes; i++) {
        const char *name = (const char *)attributes[i * 5];
        const char *ns = (const char *)attributes[i * 5 + 2];
        char *valueStart = (char *)attributes[i * 5 + 3];
        char *valueEnd = (char *)attributes[i * 5 + 4];
        const int valueLen = valueEnd - valueStart;

        if(!ns && !strncmp(name, "item", 4)) {
            name += 4;
            /* check @itemscope */
            if(!strcmp(name, "scope")) {
                isStartScope = CSEM_TRUE;
            }
            /* check @itemtype */
            else if(!strcmp(name, "type")) {
                if((error = CSEM_Utils_Strntoks(valueStart, valueLen, HTML5_SPACES, &itemtypes))) {
                    goto ERROR;
                }
            }
            /* check @itemref */
            else if(!strcmp(name, "ref")) {
                if((error = CSEM_Utils_Strntoks(valueStart, valueLen, HTML5_SPACES, &itemrefs))) {
                    goto ERROR;
                }
            }
            /* check @itemid */
            else if(!strcmp(name, "id")) {
                if(!(itemid = CSEM_Utils_Strncpy(valueStart, valueLen))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
            }
            /* check @itemprop */
            else if(!strcmp(name, "prop")) {
                if(microHandler) {
                    microHandler -> startPropValue = CSEM_TRUE;

                    /* buffering the @itemprop value */
                    if(valueLen > CSEM_ALLOWED_MAX_ATTR_VALUE_SIZE) {
                        error = CSEM_ERROR_TOO_LARGE_FIELD;
                        goto ERROR;
                    }
                    if(!(propName = CSEM_Utils_Strncpy(valueStart, valueLen))) {
                        error = CSEM_ERROR_MEMORY;
                        goto ERROR;
                    }
                }
            }
        } else if(!strcmp(name, "id")) {
            isStartId = CSEM_TRUE;
            if(!(id = CSEM_Utils_Strncpy(valueStart, valueLen))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        } else if(propAttName && !strcmp(name, propAttName)) {
            /* buffering the meta[@content] */
            if(valueLen > CSEM_ALLOWED_MAX_ATTR_VALUE_SIZE) {
                error = CSEM_ERROR_TOO_LARGE_FIELD;
                goto ERROR;
            }
            propValueLen = valueLen;
            if(!(propValue = CSEM_Utils_Strncpy(valueStart, valueLen))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        }
    }

    if(microHandler) {
        /* fire microdata handler */
        if(microHandler -> startPropValue) {/* @itemprop */
            if(microHandler -> startProp) {
                CSEM_Bool isUrlPropElement = CSEM_Micro_IsUrlPropElement((const char *)localname, (const char *)URI);
                freeProp = microHandler -> startProp(parser -> userdata, propName, isUrlPropElement);
            }
            if(propAttName) {
                if(microHandler -> itemProp) {
                    if(propValue) {
                        microHandler -> itemProp(parser -> userdata, propValue, propValueLen);
                    } else {
                        microHandler -> itemProp(parser -> userdata, "", 0);
                    }
                }
            }
        }
        if(isStartScope) {/* @itemscope */
            if(microHandler -> startScope) {
                freeScope = microHandler -> startScope(parser -> userdata,
                        itemtypes, itemrefs, itemid);
            }
        }
        if(isStartId) {/* @id */
            if(microHandler -> startId) {
                freeId = microHandler -> startId(parser -> userdata, id);
            }
        }
        /* update state of microdata */
        if(isStartScope) {
            {/* push current depth to scopeDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(microHandler -> currentDepth), sizeof(int));
                CSEM_List_Add(microHandler -> scopeDepth, bufDepth);
            }
            microHandler -> startPropValue = CSEM_FALSE;/* for @itmescope && @itemprop */
        }
        if(microHandler -> startPropValue) {
            {/* push current depth to propDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(microHandler -> currentDepth), sizeof(int));
                CSEM_List_Add(microHandler -> propDepth, bufDepth);
            }
            if(propAttName) {
                microHandler -> startPropValue = CSEM_FALSE;
            }
        }
        if(isStartId) {
            microHandler -> idDepth = microHandler -> currentDepth;
        }
    }

ERROR:
    if(error) {
        if(handler && handler -> error) {
            handler -> error(parser, error);
        }
    }
    /* dispose temporal buffer */
    CSEM_Free(propValue);

    if(freeScope) {
        CSEM_List_Dispose(itemtypes, CSEM_TRUE);
        CSEM_List_Dispose(itemrefs, CSEM_TRUE);
        CSEM_Free(itemid);
    }
    if(freeId) {
        CSEM_Free(id);
    }
    if(freeProp) {
        CSEM_Free(propName);
    }
}
static void show_list_int(CSEM_List *list) {
#ifdef CSEM_DEBUG_PARSER
    size_t size = CSEM_List_Size(list);
    int i = 0;
    int *pt = NULL;
    for(i = 0; i < size; i++) {
        pt = CSEM_List_Get(list, i);
        printf("[%d]%d,", i, *pt);
    }
    printf("\n");
#endif
}
static void sax_characters(void *ctx, const xmlChar *ch, int len) {
    CSEM_Parser *parser = ctx;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;

#ifdef CSEM_DEBUG_PARSER
    puts("CH");
    printf("current depth = %d\n", microHandler -> currentDepth);
    show_list_int(microHandler -> scopeDepth);
#endif

    if(microHandler) {
        if(microHandler -> startPropValue) {
            if(microHandler -> itemProp) {
                microHandler -> itemProp(parser -> userdata, (const char *)ch, len);
            }
        }
    }
}
static void sax_endElementNs(void *ctx, const xmlChar *localname,
        const xmlChar *prefix, const xmlChar *URI) {
    CSEM_Parser *parser = ctx;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;

    if(microHandler) {
        int scopeDepthSize = CSEM_List_Size(microHandler -> scopeDepth);
        int scopeDepthIndex = scopeDepthSize - 1;
        int *tmpScopeDepth = CSEM_List_Get(microHandler -> scopeDepth, scopeDepthIndex);

        int propDepthSize = CSEM_List_Size(microHandler -> propDepth);
        int propDepthIndex = propDepthSize - 1;
        int *tmpPropDepth = CSEM_List_Get(microHandler -> propDepth, propDepthIndex);

        show_list_int(microHandler -> scopeDepth);

        if(*tmpScopeDepth == microHandler -> currentDepth) {
            if(microHandler -> endScope) {
                microHandler -> endScope(parser -> userdata);
            }
            if(scopeDepthIndex) {
                CSEM_Free(CSEM_List_Get(microHandler -> scopeDepth, scopeDepthIndex));
                CSEM_List_Remove(microHandler -> scopeDepth, scopeDepthIndex);
            }
        }
        if(tmpPropDepth && *tmpPropDepth == microHandler -> currentDepth) {
            if(microHandler -> endProp) {
                microHandler -> endProp(parser -> userdata);
            }
            microHandler -> startPropValue = CSEM_FALSE;

            if(propDepthIndex) {
                CSEM_Free(CSEM_List_Get(microHandler -> propDepth, propDepthIndex));
                CSEM_List_Remove(microHandler -> propDepth, propDepthIndex);
            }
        }
        if(microHandler -> idDepth == microHandler -> currentDepth) {
            if(microHandler && microHandler -> endId) {
                microHandler -> endId(parser -> userdata);
            }
            microHandler -> idDepth = -1;
        }

        {/* update state */
#ifdef CSEM_DEBUG_PARSER
            printf("</%s>%d\n", localname, microHandler -> currentDepth);
#endif
            microHandler -> currentDepth--;
        }
    }
}
static void sax_fatalError(void *ctx, const char *msg, ...) {

}
static CSEM_Error csem_sax_init(CSEM_Parser *parser) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* init SAX2 */
    memset(&(parser -> sax), 0, sizeof(xmlSAXHandler));
    parser -> sax.initialized = XML_SAX2_MAGIC;
    parser -> sax.startElementNs = sax_startElementNs;
    parser -> sax.endElementNs = sax_endElementNs;
    parser -> sax.characters = sax_characters;
    parser -> sax.fatalError = sax_fatalError;

    if(!(parser -> ctxt = xmlCreatePushParserCtxt(&(parser -> sax), parser, NULL, 0, NULL))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    parser -> ctxt -> recovery = 1;
    return error;
ERROR:
    xmlFreeParserCtxt(parser -> ctxt);
    return error;
}
static CSEM_Error csem_parser_init(CSEM_Parser *parser) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* init SAX2 */
    if((error = csem_sax_init(parser))) {
        goto ERROR;
    }
    return error;
ERROR:
    return error;
}
CSEM_Error CSEM_Parser_Parse(CSEM_Parser *parser, int fd) {
    CSEM_Error error = CSEM_ERROR_NONE;
    size_t readLen = 0;
    size_t bufsize = sizeof(parser -> buf);

    while((readLen = read(fd, parser -> buf, bufsize))) {
        if((error = CSEM_Parser_ParseChunk(parser, parser -> buf,
                readLen, 0))) {
            goto ERROR;
        }
    }
    if((error = CSEM_Parser_ParseChunk(parser, parser -> buf, 0, 1))) {
        goto ERROR;
    }
    return error;
ERROR:
    return error;
}
CSEM_Error CSEM_Parser_ParseChunk(CSEM_Parser *parser, const char *chunk, int size, int terminate) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(xmlParseChunk(parser -> ctxt, chunk, size, terminate)) {
        error = CSEM_ERROR_Parse;
    }
    return error;
}
void CSEM_Parser_Dispose(CSEM_Parser *parser) {
    if(parser) {
        /* dispose SAX2 */
        xmlFreeParserCtxt(parser -> ctxt);
        xmlCleanupParser();
        CSEM_Free(parser);
    }
}
CSEM_Error CSEM_Parser_Create(CSEM_Parser **parser) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Parser *tmp_parser = NULL;
    if(!(tmp_parser = CSEM_Calloc(1, sizeof(CSEM_Parser)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    if((error = csem_parser_init(tmp_parser))) {
        goto ERROR;
    }

    /* result */
    *parser = tmp_parser;
    return error;

ERROR:
    CSEM_Parser_Dispose(tmp_parser);
    return error;
}
void CSEM_Parser_SetUserdata(CSEM_Parser *parser, void *userdata) {
    parser -> userdata = userdata;
}
void CSEM_Parser_SetHandler(CSEM_Parser *parser, CSEM_Handler *handler) {
    parser -> handler = handler;
}
CSEM_Error CSEM_Handler_Create(CSEM_Handler **handler) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Handler *result = NULL;

    if(!(result = CSEM_Calloc(1, sizeof(CSEM_Handler)))) {
        error = CSEM_ERROR_MEMORY;
        goto FINISH;
    }
    /* result */
    *handler = result;
FINISH:
    return error;
}
void CSEM_Handler_Dispose(CSEM_Handler *handler, CSEM_Bool doFree) {
    if(handler) {
        if(doFree) {
            if(handler -> microdata) {
                CSEM_Micro_DisposeHandler(handler -> microdata);
            }
            if(handler -> rdfa_lite) {
                CSEM_RDFaLite_DisposeHandler(handler -> rdfa_lite);
            }
        }
        CSEM_Free(handler);
    }
}
void CSEM_Handler_SetErrorHandler(CSEM_Handler *handler, CSEM_Parser_ErrorHandler error) {
    handler -> error = error;
}
void CSEM_Handler_SetMicrodataHandler(CSEM_Handler *handler, CSEM_Micro_Handlers *microdata) {
    handler -> microdata = microdata;
}
void CSEM_Handler_SetRDFaLiteHandler(CSEM_Handler *handler, CSEM_RDFaLite_Handlers *rdfa_lite) {
    handler -> rdfa_lite = rdfa_lite;
}
