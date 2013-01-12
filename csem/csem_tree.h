/* $Id$ */
#ifndef CSEM_TREE_H
#define CSEM_TREE_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem_common.h"
#include "csem_list.h"

typedef enum CSEM_NODE_TYPE {
    CSEM_NODE_TYPE_NONE = 0,
    CSEM_NODE_TYPE_DOCUMENT,
    CSEM_NODE_TYPE_MICRO_ITEM,
    CSEM_NODE_TYPE_MICRO_ID,
    CSEM_NODE_TYPE_MICRO_PROPERTY
} CSEM_NODE_TYPE;

typedef struct CSEM_Document CSEM_Document;
typedef struct CSEM_Node CSEM_Node;


CSEM_Error     CSEM_Node_Create(CSEM_Node **node, CSEM_NODE_TYPE type, void *obj, CSEM_Node *parent);
CSEM_NODE_TYPE CSEM_Node_GetType(CSEM_Node *node);
CSEM_Node     *CSEM_Node_GetParent(CSEM_Node *node);
void          *CSEM_Node_GetObject(CSEM_Node *node);
void           CSEM_Node_Dispose(CSEM_Node *node);

CSEM_Error CSEM_Document_Create(CSEM_Document **document);
void       CSEM_Document_Dispose(CSEM_Document *document);
CSEM_List *CSEM_Document_GetChildren(CSEM_Document *doc);
CSEM_Error CSEM_Document_AppendChild(CSEM_Document *doc, CSEM_Node *child);

CSEM_NS_C_END

#endif /* CSEM_TREE_H */
