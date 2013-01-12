/* $Id$ */
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

#include <libxml/SAX2.h>

#include "csem/csem_stream.h"
#include "csem/csem_micro_stream.h"
#include "csem/csem_rdfa_lite.h"
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

struct CSEM_Micro_Handlers {
    CSEM_Micro_StartScope startScope;
    CSEM_Micro_EndScope   endScope;
    CSEM_Micro_StartProp  startProp;
    CSEM_Micro_ItemProp   itemProp;
    CSEM_Micro_EndProp    endProp;
    CSEM_Micro_StartId    startId;
    CSEM_Micro_EndId      endId;

    int currentDepth;
    /* scope */
    CSEM_List *scopeDepth;
    /* id */
    int idDepth;
    /* prop */
    CSEM_Bool startPropValue;
    CSEM_List *propDepth;
};

struct CSEM_RDFaLite_Handlers {
    CSEM_RDFaLite_Handler_StartScope startScope;
    CSEM_RDFaLite_Handler_ItemProp   itemProp;
    CSEM_RDFaLite_Handler_EndScope   endScope;
};

struct CSEM_Handler {
    CSEM_Micro_Handlers *microdata;
    CSEM_RDFaLite_Handlers  *rdfa_lite;
    CSEM_Parser_ErrorHandler error;
};

struct CSEM_Parser {
    char buf[CSEM_DEFAULT_BUF_SIZE];
    void *userdata;
    CSEM_Handler *handler;

    /* underlying SAX parser */
    xmlSAXHandler sax;
    xmlParserCtxt *ctxt;
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
    char *id;
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
    CSEM_MICRO_VALUE_TYPE propValueType;
};

CSEM_NS_C_END

#endif /* CSEM_TYPES_H */
