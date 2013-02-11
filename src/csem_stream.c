/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <string.h>
#include <unistd.h>

#include "csem_utils.h"
#include "csem_types.h"
#include "csem_micro_stream_internal.h"
#include "libxml2/libxml/HTMLparser.h"

const char *HTML5_SPACES = "\x20\x09\x0a\x0c\x0d";

/* #define CSEM_DEBUG_PARSER */

typedef struct {
    CSEM_Bool isStartScope;
    CSEM_Bool isStartId;
    CSEM_List *itemtypes;
    CSEM_List *itemrefs;
    char *propName;
    char *propAttName;
    char *propValue;
    char *itemid;
    char *id;
    CSEM_Bool freeScope;
    CSEM_Bool freeId;
    CSEM_Bool freeProp;
} Micro_StartElement;

typedef struct {
    CSEM_Bool isStartScope;
    CSEM_Bool isStartVocab;
    CSEM_Bool isStartPrefix;
    CSEM_List *itemtypes;
    CSEM_List *prefixes;
    char *vocab;
    char *propName;
    char *propAttName;
    char *propValue;
    char *resource;
    CSEM_Bool freeScope;
    CSEM_Bool freeProp;
} RDFa_StartElement;

static void init_micro_startElement(Micro_StartElement *content) {
    memset(content, 0, sizeof(Micro_StartElement));
    content -> freeScope = CSEM_TRUE;
    content -> freeId = CSEM_TRUE;
    content -> freeProp = CSEM_TRUE;
}
static void init_rdfa_startElement(RDFa_StartElement *content) {
    memset(content, 0, sizeof(RDFa_StartElement));
    content -> freeScope = CSEM_TRUE;
    content -> freeProp = CSEM_TRUE;
}
static CSEM_Error rdfa_curie_resolve(CSEM_NSManager *nsManager, CSEM_List *values) {
    CSEM_Error error = CSEM_ERROR_NONE;

    int i = 0;
    for(i = 0; i < CSEM_List_Size(values); i++) {
        char *value = CSEM_List_Get(values, i);
        CSEM_List *prefix_term = NULL;
        if((error = CSEM_Utils_Strtoks(value, ":", &prefix_term))) {
            goto FINISH;
        }
        if(CSEM_List_Size(prefix_term) == 1) {/* in case of only term */
            char *term = CSEM_List_Get(prefix_term, 0);
            const char *vocab = CSEM_NSManager_lookupURI(nsManager, NULL);
            if(vocab) {
                size_t newValueLen = strlen(vocab) + strlen(term) + 2;
                char *newValue = NULL;
                if(!(newValue = CSEM_Calloc(1, newValueLen))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                CSEM_Utils_Strcat(newValue, newValueLen, 2, vocab, term);
                value = CSEM_List_Replace(values, i, newValue);
                CSEM_Free(value);
            }

        } else {/* in case of prefix : term */
            char *prefix = CSEM_List_Get(prefix_term, 0);
            char *term = CSEM_List_Get(prefix_term, 1);
            const char *uri = CSEM_NSManager_lookupURI(nsManager, prefix);
            if(uri) {
                size_t newValueLen = strlen(uri) + strlen(term) + 2;
                char *newValue = NULL;
                if(!(newValue = CSEM_Calloc(1, newValueLen))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                CSEM_Utils_Strcat(newValue, newValueLen, 2, uri, term);
                value = CSEM_List_Replace(values, i, newValue);
                CSEM_Free(value);
            }
        }
        CSEM_List_Dispose(prefix_term, CSEM_TRUE);
        continue;
ERROR:
        CSEM_List_Dispose(prefix_term, CSEM_TRUE);
        break;
    }
FINISH:
    return error;
}
static void sax_startElement(void *ctx, const xmlChar *name, const xmlChar **atts) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Parser *parser = ctx;
    int i = 0;
    Micro_StartElement micro;
    RDFa_StartElement rdfa;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;
    CSEM_RDFaLite_Handlers *rdfaHandler = handler ? handler -> rdfa : NULL;

    /* init */
    init_micro_startElement(&micro);
    init_rdfa_startElement(&rdfa);

    /* init state */
    if(microHandler) {
        (microHandler -> currentDepth)++;
        microHandler -> startPropValue = CSEM_FALSE;
        micro.propAttName = CSEM_Micro_GetAttNameForPropValue((const char *)name, NULL);
    }
    if(rdfaHandler) {
        (rdfaHandler -> currentDepth)++;
        rdfaHandler -> startPropValue = CSEM_FALSE;
        rdfa.propAttName = CSEM_Micro_GetAttNameForPropValue((const char *)name, NULL);
    }

#ifdef CSEM_DEBUG_PARSER
    printf("<%s>%d\n", name, microHandler -> currentDepth);
#endif

    i =0;
    while(atts && atts[i]) {
        char *name = (char *)atts[i++];
        char *value = (char *)atts[i++];
#ifdef CSEM_DEBUG_PARSER
        printf("\t%s=%s\n", name, value);
#endif

        /* check @itemscope */
        if(!strcmp(name, "itemscope")) {
            micro.isStartScope = CSEM_TRUE;
        }
        /* check @itemtype */
        else if(!strcmp(name, "itemtype")) {
            if((error = CSEM_Utils_Strtoks(value, HTML5_SPACES, &(micro.itemtypes)))) {
                goto ERROR;
            }
        }
        /* check @itemref */
        else if(!strcmp(name, "itemref")) {
            if((error = CSEM_Utils_Strtoks(value, HTML5_SPACES, &(micro.itemrefs)))) {
                goto ERROR;
            }
        }
        /* check @itemid */
        else if(!strcmp(name, "itemid")) {
            if(!(micro.itemid = CSEM_Utils_Strcpy(value))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        }
        /* check @itemprop */
        else if(!strcmp(name, "itemprop")) {
            if(microHandler) {
                microHandler -> startPropValue = CSEM_TRUE;

                if(!(micro.propName = CSEM_Utils_Strcpy(value))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
            }
        } else if(!strcmp(name, "id")) {
            micro.isStartId = CSEM_TRUE;
            if(!(micro.id = CSEM_Utils_Strcpy(value))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        } else if(!strcmp(name, "vocab")) {
            rdfa.isStartVocab = CSEM_TRUE;
            if(!(rdfa.vocab = CSEM_Utils_Strcpy(value))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        } else if(!strcmp(name, "prefix")) {
            rdfa.isStartPrefix = CSEM_TRUE;
            if((error = CSEM_Utils_Strtoks(value, HTML5_SPACES, &(rdfa.prefixes)))) {
                goto ERROR;
            }
        } else if(!strcmp(name, "typeof")) {
            rdfa.isStartScope = CSEM_TRUE;
            if((error = CSEM_Utils_Strtoks(value, HTML5_SPACES, &(rdfa.itemtypes)))) {
                goto ERROR;
            }
        } else if(!strcmp(name, "resource")) {
            rdfa.isStartScope = CSEM_TRUE;
            if(!(rdfa.resource = CSEM_Utils_Strcpy(value))) {
                error = CSEM_ERROR_MEMORY;
                goto ERROR;
            }
        } else if(!strcmp(name, "property")) {
            if(rdfaHandler) {
                rdfaHandler -> startPropValue = CSEM_TRUE;

                if(!(rdfa.propName = CSEM_Utils_Strcpy(value))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
            }
        }
        else {
            if(micro.propAttName && !strcmp(name, micro.propAttName)) {
                if(!(micro.propValue = CSEM_Utils_Strcpy(value))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
            }
            if(rdfa.propAttName && !strcmp(name, rdfa.propAttName)) {
                if(!(rdfa.propValue = CSEM_Utils_Strcpy(value))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
            }
        }
    }
    if(microHandler) {
        /* fire microdata handler */
        if(microHandler -> startPropValue) {/* @itemprop */
            if(microHandler -> startProp) {
                CSEM_Bool isUrlPropElement = CSEM_Micro_IsUrlPropElement((const char *)name, NULL);
                micro.freeProp = microHandler -> startProp(parser -> userdata, micro.propName, isUrlPropElement);
            }
            if(micro.propAttName) {
                if(microHandler -> itemProp) {
                    if(micro.propValue) {
                        microHandler -> itemProp(parser -> userdata, micro.propValue, strlen(micro.propValue));
                    } else {
                        microHandler -> itemProp(parser -> userdata, "", 0);
                    }
                }
            }
        }
        if(micro.isStartScope) {/* @itemscope */
            if(microHandler -> startScope) {
                micro.freeScope = microHandler -> startScope(parser -> userdata,
                        micro.itemtypes, micro.itemrefs, micro.itemid);
            }
        }
        if(micro.isStartId) {/* @id */
            if(microHandler -> startId) {
                micro.freeId = microHandler -> startId(parser -> userdata, micro.id);
            }
        }
        /* update state of microdata */
        if(microHandler -> startPropValue) {
            {/* push current depth to propDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(microHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(microHandler -> propDepth, bufDepth);
            }
            if(micro.propAttName) {
                microHandler -> startPropValue = CSEM_FALSE;
            }
        }
        if(micro.isStartScope) {
            {/* push current depth to scopeDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(microHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(microHandler -> scopeDepth, bufDepth);
            }
            microHandler -> startPropValue = CSEM_FALSE;/* for @itmescope && @itemprop */
        }
        if(micro.isStartId) {
            {/* push current depth to idDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(microHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(microHandler -> idDepth, bufDepth);
            }
        }
    }
    if(rdfaHandler) {
        /* process vocab, prefix */
        if(rdfa.vocab || rdfa.prefixes) {
            if(rdfa.vocab) {
                if((error = CSEM_NSManager_Add(rdfaHandler -> nsManager, NULL, rdfa.vocab))) {
                    goto ERROR;
                }
            }
            if(rdfa.prefixes) {
                int i = 0;
                for(i = 0; i < CSEM_List_Size(rdfa.prefixes); i += 2) {
                    char *prefix = CSEM_List_Get(rdfa.prefixes, i);
                    char *uri = CSEM_List_Get(rdfa.prefixes, i + 1);
                    prefix[strlen(prefix) - 1] = '\0';/* cut ':' */
                    if((error = CSEM_NSManager_Add(rdfaHandler -> nsManager, prefix, uri))) {
                        goto ERROR;
                    }
                }
            }
            if((error = CSEM_NSManager_Push(rdfaHandler -> nsManager))) {
                goto ERROR;
            }
        }
        if(rdfa.itemtypes) {
            if((error = rdfa_curie_resolve(rdfaHandler -> nsManager, rdfa.itemtypes))) {
                goto ERROR;
            }
        }
        /* fire RDFa handler */
        if(rdfaHandler -> startPropValue) {/* @property */
            if(rdfaHandler -> startProp) {
                CSEM_Bool isUrlPropElement = CSEM_Micro_IsUrlPropElement((const char *)name, NULL);
                rdfa.freeProp = rdfaHandler -> startProp(parser -> userdata, rdfa.propName, isUrlPropElement);
            }
            if(rdfa.propAttName) {
                if(rdfaHandler -> itemProp) {
                    if(rdfa.propValue) {
                        rdfaHandler -> itemProp(parser -> userdata, rdfa.propValue, strlen(rdfa.propValue));
                    } else {
                        rdfaHandler -> itemProp(parser -> userdata, "", 0);
                    }
                }
            }
        }
        if(rdfa.isStartScope) {/* @typeof | @resource */
            if(rdfaHandler -> startScope) {
                rdfa.freeScope = rdfaHandler -> startScope(parser -> userdata,
                        rdfa.resource, rdfa.itemtypes);
            }
        }
        /* update state of rdfa */
        if(rdfaHandler -> startPropValue) {
            {/* push current depth to propDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(rdfaHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(rdfaHandler -> propDepth, bufDepth);
            }
            if(rdfa.propAttName) {
                rdfaHandler -> startPropValue = CSEM_FALSE;
            }
        }
        if(rdfa.isStartScope) {
            {/* push current depth to scopeDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(rdfaHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(rdfaHandler -> scopeDepth, bufDepth);
            }
            /* for @property && (@typeof || @resource) */
            rdfaHandler -> startPropValue = CSEM_FALSE;
        }
        if(rdfa.isStartVocab) {
            {/* push current depth to vocabDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(rdfaHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(rdfaHandler -> vocabDepth, bufDepth);
            }
        }
        if(rdfa.isStartPrefix) {
            {/* push current depth to prefixDepth */
                int *bufDepth = NULL;
                if(!(bufDepth = CSEM_Malloc(sizeof(int)))) {
                    error = CSEM_ERROR_MEMORY;
                    goto ERROR;
                }
                memcpy(bufDepth, &(rdfaHandler -> currentDepth), sizeof(int));
                CSEM_Stack_Push(rdfaHandler -> prefixDepth, bufDepth);
            }
        }
    }

ERROR:
    if(error) {
        if(handler && handler -> error) {
            handler -> error(parser, error);
        }
    }
    /* dispose temporal buffer */
    CSEM_Free(micro.propValue);
    CSEM_Free(rdfa.propValue);
    CSEM_List_Dispose(rdfa.prefixes, CSEM_FALSE);

    if(micro.freeScope) {
        CSEM_List_Dispose(micro.itemtypes, CSEM_TRUE);
        CSEM_List_Dispose(micro.itemrefs, CSEM_TRUE);
        CSEM_Free(micro.itemid);
    }
    if(micro.freeId) {
        CSEM_Free(micro.id);
    }
    if(micro.freeProp) {
        CSEM_Free(micro.propName);
    }
    if(rdfa.freeScope) {
        CSEM_List_Dispose(rdfa.itemtypes, CSEM_TRUE);
        CSEM_Free(rdfa.resource);
    }
    if(rdfa.freeProp) {
        CSEM_Free(rdfa.propName);
    }
}
#ifdef CSEM_DEBUG_PARSER
static void show_list_int(CSEM_List *list) {

    size_t size = CSEM_List_Size(list);
    int i = 0;
    int *pt = NULL;
    for(i = 0; i < size; i++) {
        pt = CSEM_List_Get(list, i);
        printf("[%d]%d,", i, *pt);
    }
    printf("\n");
}
#endif
static void sax_characters(void *ctx, const xmlChar *ch, int len) {
    CSEM_Parser *parser = ctx;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;
    CSEM_RDFaLite_Handlers *rdfaHandler = handler ? handler -> rdfa : NULL;

#ifdef CSEM_DEBUG_PARSER
    puts("CH");
    if(microHandler) {
        printf("current depth = %d\n", microHandler -> currentDepth);
        show_list_int(microHandler -> scopeDepth);
    } else if(rdfaHandler) {
        printf("current depth = %d\n", rdfaHandler -> currentDepth);
        show_list_int(rdfaHandler -> scopeDepth);
    }
#endif

    if(microHandler) {
        if(microHandler -> startPropValue) {
            if(microHandler -> itemProp) {
                microHandler -> itemProp(parser -> userdata, (const char *)ch, len);
            }
        }
    }
    if(rdfaHandler) {
        if(rdfaHandler -> startPropValue) {
            if(rdfaHandler -> itemProp) {
                rdfaHandler -> itemProp(parser -> userdata, (const char *)ch, len);
            }
        }
    }
}
static void sax_endElement(void *ctx, const xmlChar *name) {
    CSEM_Parser *parser = ctx;
    CSEM_Handler *handler = parser -> handler;
    CSEM_Micro_Handlers *microHandler = handler ? handler -> microdata : NULL;
    CSEM_RDFaLite_Handlers *rdfaHandler = handler ? handler -> rdfa : NULL;

    if(microHandler) {
        int *tmpScopeDepth = CSEM_Stack_Top(microHandler -> scopeDepth);
        int *tmpIdDepth = CSEM_Stack_Top(microHandler -> idDepth);
        int *tmpPropDepth = CSEM_Stack_Top(microHandler -> propDepth);
#ifdef CSEM_DEBUG_PARSER
        show_list_int(microHandler -> scopeDepth);
#endif

        if(*tmpScopeDepth == microHandler -> currentDepth) {
            if(microHandler -> endScope) {
                microHandler -> endScope(parser -> userdata);
            }
            if(CSEM_Stack_Size(microHandler -> scopeDepth) > 1) {
                void *depth = CSEM_Stack_Pop(microHandler -> scopeDepth);
                CSEM_Free(depth);
            }
        }
        if(*tmpIdDepth == microHandler -> currentDepth) {
            if(microHandler -> endId) {
                microHandler -> endId(parser -> userdata);
            }
            if(CSEM_Stack_Size(microHandler -> idDepth) > 1) {
                void *depth = CSEM_Stack_Pop(microHandler -> idDepth);
                CSEM_Free(depth);
            }
        }
        if(*tmpPropDepth == microHandler -> currentDepth) {
            if(microHandler -> endProp) {
                microHandler -> endProp(parser -> userdata);
            }
            microHandler -> startPropValue = CSEM_FALSE;

            if(CSEM_Stack_Size(microHandler -> propDepth) > 1) {
                void *depth = CSEM_Stack_Pop(microHandler -> propDepth);
                CSEM_Free(depth);
            }
        }

        {/* update state */
#ifdef CSEM_DEBUG_PARSER
            printf("</%s>%d\n", name, microHandler -> currentDepth);
#endif
            microHandler -> currentDepth--;
        }
    }
    if(rdfaHandler) {
        int *tmpScopeDepth = CSEM_Stack_Top(rdfaHandler -> scopeDepth);
        int *tmpVocabDepth = CSEM_Stack_Top(rdfaHandler -> vocabDepth);
        int *tmpPrefixDepth = CSEM_Stack_Top(rdfaHandler -> prefixDepth);
        int *tmpPropDepth = CSEM_Stack_Top(rdfaHandler -> propDepth);

#ifdef CSEM_DEBUG_PARSER
        show_list_int(rdfaHandler -> scopeDepth);
#endif

        if(*tmpScopeDepth == rdfaHandler -> currentDepth) {
            if(rdfaHandler -> endScope) {
                rdfaHandler -> endScope(parser -> userdata);
            }
            if(CSEM_Stack_Size(rdfaHandler -> scopeDepth) > 1) {
                void *depth = CSEM_Stack_Pop(rdfaHandler -> scopeDepth);
                CSEM_Free(depth);
            }
        }
        if(*tmpVocabDepth == rdfaHandler -> currentDepth) {
            /* TODO */
            CSEM_NSManager_Pop(rdfaHandler -> nsManager, CSEM_TRUE);
            if(CSEM_Stack_Size(rdfaHandler -> vocabDepth) > 1) {
                void *depth = CSEM_Stack_Pop(rdfaHandler -> vocabDepth);
                CSEM_Free(depth);
            }
        }
        if(*tmpPrefixDepth == rdfaHandler -> currentDepth) {
            /* TODO : update prefix manager */
            CSEM_NSManager_Pop(rdfaHandler -> nsManager, CSEM_TRUE);
            if(CSEM_Stack_Size(rdfaHandler -> prefixDepth) > 1) {
                void *depth = CSEM_Stack_Pop(rdfaHandler -> prefixDepth);
                CSEM_Free(depth);
            }
        }
        if(*tmpPropDepth == rdfaHandler -> currentDepth) {
            if(rdfaHandler -> endProp) {
                rdfaHandler -> endProp(parser -> userdata);
            }
            rdfaHandler -> startPropValue = CSEM_FALSE;

            if(CSEM_Stack_Size(rdfaHandler -> propDepth) > 1) {
                void *depth = CSEM_Stack_Pop(rdfaHandler -> propDepth);
                CSEM_Free(depth);
            }
        }

        {/* update state */
#ifdef CSEM_DEBUG_PARSER
            printf("</%s>%d\n", name, rdfaHandler -> currentDepth);
#endif
            rdfaHandler -> currentDepth--;
        }
    }
}
static void sax_fatalError(void *ctx, const char *msg, ...) {

}
static CSEM_Error csem_sax_init(CSEM_Parser *parser) {
    CSEM_Error error = CSEM_ERROR_NONE;

    /* init SAX2 */
    memset(&(parser -> sax), 0, sizeof(htmlSAXHandler));
    parser -> sax.startElement = sax_startElement;
    parser -> sax.characters = sax_characters;
    parser -> sax.endElement = sax_endElement;
    parser -> sax.fatalError = sax_fatalError;

    if(!(parser -> ctxt = htmlCreatePushParserCtxt(&(parser -> sax), parser, NULL, 0, NULL, 0))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    htmlCtxtUseOptions(parser -> ctxt,
            HTML_PARSE_RECOVER  |
            HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

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
    int libxmlError = 0;
    if((libxmlError = htmlParseChunk(parser -> ctxt, chunk, size, terminate))){
        if(libxmlError == XML_ERR_INTERNAL_ERROR
                || libxmlError == XML_ERR_NO_MEMORY) {
            error = CSEM_ERROR_PARSE;
        }
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
            if(handler -> rdfa) {
                CSEM_RDFaLite_DisposeHandler(handler -> rdfa);
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
void CSEM_Handler_SetRDFaLiteHandler(CSEM_Handler *handler, CSEM_RDFaLite_Handlers *rdfa) {
    handler -> rdfa = rdfa;
}
