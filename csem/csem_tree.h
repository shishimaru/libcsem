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

#include "csem_list.h"

/**
 * Types of the tree nodes.
 */
typedef enum CSEM_NODE_TYPE {
    CSEM_NODE_TYPE_NONE = 0,
    /** document node */
    CSEM_NODE_TYPE_DOCUMENT,
    /** item node of microdata */
    CSEM_NODE_TYPE_MICRO_ITEM,
    /** id node of microdata */
    CSEM_NODE_TYPE_MICRO_ID,
    /** property node of microdata */
    CSEM_NODE_TYPE_MICRO_PROPERTY
} CSEM_NODE_TYPE;
/**
 * Root node of the tree.
 */
typedef struct CSEM_Document CSEM_Document;
/**
 * Abstract node of the tree nodes.
 */
typedef struct CSEM_Node CSEM_Node;
/**
 * Create a node.
 * @param node   [out]created node
 * @param type   [in]node type
 * @param obj    [in]content object related to the node
 * @param parent [in]parent node
 * @return error code
 */
CSEM_Error CSEM_Node_Create(CSEM_Node **node, CSEM_NODE_TYPE type, void *obj, CSEM_Node *parent);
/**
 * Get node type.
 * @param node [in]node
 * @return node type
 */
CSEM_NODE_TYPE CSEM_Node_GetType(CSEM_Node *node);
/**
 * Get parent node.
 * @param node [in]node
 * @return parent node
 */
CSEM_Node *CSEM_Node_GetParent(CSEM_Node *node);
/**
 * Get content object related to the specified node.
 * @param node [in]node
 */
void *CSEM_Node_GetObject(CSEM_Node *node);
/**
 * Dispose the node.
 * @param node [in]node to be freed
 */
void CSEM_Node_Dispose(CSEM_Node *node);
/**
 * Create document node.
 * @param document [out]create document node
 * @return error code
 */
CSEM_Error CSEM_Document_Create(CSEM_Document **document);
/**
 * Dispose document node.
 * @param document [in]document node to be freed
 */
void CSEM_Document_Dispose(CSEM_Document *document);
/**
 * Get child nodes.
 * @param doc [in]document node
 * @return child node list
 */
CSEM_List *CSEM_Document_GetChildren(CSEM_Document *doc);
/**
 * Append the specified node as child node under the document node.
 * @param doc   [in]document node
 * @param child [in]child node to be appended
 * @return error node
 */
CSEM_Error CSEM_Document_AppendChild(CSEM_Document *doc, CSEM_Node *child);

CSEM_NS_C_END

#endif /* CSEM_TREE_H */
