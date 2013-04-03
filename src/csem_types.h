/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_TYPES_H
#define CSEM_TYPES_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include <libxml/HTMLparser.h>

#include "csem/csem_stream.h"
#include "csem/csem_micro_stream.h"
#include "csem/csem_rdfa_stream.h"
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"
#include "csem/csem_url.h"
#include "csem_stack.h"
#include "csem_ns_manager.h"

struct CSEM_Micro_Handlers {
    CSEM_Micro_ItemStart itemStart;
    CSEM_Micro_ItemEnd   itemEnd;
    CSEM_Micro_PropStart propStart;
    CSEM_Micro_PropValue propValue;
    CSEM_Micro_PropEnd   propEnd;
    CSEM_Micro_IdStart   idStart;
    CSEM_Micro_IdEnd     idEnd;

    int currentDepth;
    /* item */
    CSEM_Stack *itemDepth;
    /* id */
    CSEM_Stack *idDepth;
    /* prop */
    CSEM_Bool startPropValue;
    CSEM_Bool isPropValue;
    CSEM_Stack *propDepth;
};

struct CSEM_RDFa_Handlers {
    CSEM_RDFa_ItemStart itemStart;
    CSEM_RDFa_ItemEnd   itemEnd;
    CSEM_RDFa_PropStart propStart;
    CSEM_RDFa_PropValue propValue;
    CSEM_RDFa_PropEnd   propEnd;

    CSEM_NSManager *nsManager;
    int currentDepth;
    /* item */
    CSEM_Stack *itemDepth;
    /* vocab */
    CSEM_Stack *vocabDepth;
    /* prefix */
    CSEM_Stack *prefixDepth;
    /* prop */
    CSEM_Bool startPropValue;
    CSEM_Bool isPropValue;
    CSEM_Stack *propDepth;
};

struct CSEM_Handler {
    CSEM_Micro_Handlers *microdata;
    CSEM_RDFa_Handlers  *rdfa;
    CSEM_Parser_ErrorHandler error;
};

struct CSEM_Parser {
    char buf[CSEM_DEFAULT_BUF_SIZE];
    void *userdata;
    CSEM_Handler *handler;
    CSEM_Url *baseURL;

    /* underlying SAX parser */
    htmlSAXHandler sax;
    htmlParserCtxt *ctxt;
};

struct CSEM_Node {
    CSEM_Node *parent;
    CSEM_NODE_TYPE type;
    union {
        CSEM_Document *doc;
        CSEM_Item *item;
        CSEM_Id *id;
        CSEM_Property *property;
    } obj;
};

struct CSEM_Document {
    CSEM_Node *node;
    CSEM_List *children;
};
struct CSEM_Item {
    CSEM_Node *node;
    CSEM_Url *id;
    CSEM_List *types;
    CSEM_List *refs;
    CSEM_List *props;
};
struct CSEM_Id {
    CSEM_Node *node;
    char *id;
    CSEM_List *props;
};

struct CSEM_Property {
    CSEM_Node *node;
    CSEM_List *names;
    CSEM_List *values;
    CSEM_List *valueTypes;
};

struct CSEM_Builder {
    CSEM_Parser *parser;
    CSEM_Handler *handler;

    /* state manager in document building */
    CSEM_Document *document;
    CSEM_Node *activeNode;
    CSEM_Error error;
    /* prop value */
    char *buf;
    size_t bufLen;
    size_t bufReadLen;
    CSEM_VALUE_TYPE propValueType;
};

CSEM_NS_C_END

#endif /* CSEM_TYPES_H */
