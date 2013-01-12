/* $Id$ */
#ifndef CSEM_MICRO_TREE_H
#define CSEM_MICRO_TREE_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem_common.h"
#include "csem_tree.h"

typedef struct CSEM_Item CSEM_Item;
typedef struct CSEM_Id CSEM_Id;
typedef struct CSEM_Property CSEM_Property;

typedef enum {
    CSEM_MICRO_VALUE_TYPE_STR = 0,
    CSEM_MICRO_VALUE_TYPE_URL = 1,
    CSEM_MICRO_VALUE_TYPE_ITEM = 2,
    CSEM_MICRO_VALUE_TYPE_PROPERTY = 3
} CSEM_MICRO_VALUE_TYPE;

CSEM_Error CSEM_Micro_Item_Create(CSEM_Item **item);
void       CSEM_Micro_Item_Dispose(CSEM_Item *item);
char      *CSEM_Micro_Item_GetId(CSEM_Item *item);
CSEM_Error CSEM_Micro_Item_SetId(CSEM_Item *item, const char *id, CSEM_Bool doFree);
CSEM_List *CSEM_Micro_Item_GetTypes(const CSEM_Item *item);
CSEM_Error CSEM_Micro_Item_SetTypes(CSEM_Item *item, const CSEM_List *types, CSEM_Bool doFree);
CSEM_List *CSEM_Micro_Item_GetRefs(CSEM_Item *item);
CSEM_Error CSEM_Micro_Item_SetRefs(CSEM_Item *item, const CSEM_List *refs, CSEM_Bool doFree);
CSEM_List *CSEM_Micro_Item_GetProperties(const CSEM_Item *item);
CSEM_Error CSEM_Micro_Item_GetNamedPropertes(const CSEM_Item *item, const char *name, CSEM_List **properties);
CSEM_Error CSEM_Micro_Item_AddProperty(CSEM_Item *item, CSEM_Property *property, CSEM_Bool changeParent);

CSEM_Error CSEM_Micro_Id_Create(CSEM_Id **id, const char *value);
void       CSEM_Micro_Id_Dispose(CSEM_Id *id);
CSEM_List *CSEM_Micro_Id_GetProperties(CSEM_Id *id);
CSEM_Error CSEM_Micro_Id_AddProperty(CSEM_Id *id, CSEM_Property *property);
char      *CSEM_Micro_Id_GetId(CSEM_Id *id);

CSEM_Error CSEM_Micro_Property_Create(CSEM_Property **property, CSEM_List *names);
void       CSEM_Micro_Property_Dispose(CSEM_Property *property);
CSEM_List *CSEM_Micro_Property_GetNames(CSEM_Property *property);
CSEM_Error CSEM_Micro_Property_GetValues(CSEM_Property *property, CSEM_List **values, CSEM_List **valueTypes);
CSEM_Error CSEM_Micro_Property_AddValues(CSEM_Property *property, void *value, CSEM_MICRO_VALUE_TYPE type);
CSEM_Error CSEM_Micro_Properties_GetValues(CSEM_List *properties, CSEM_List **values, CSEM_List **valueTypes);

CSEM_Error CSEM_Micro_Document_GetItems(CSEM_Document *doc, CSEM_List *types, CSEM_List **items);

CSEM_NS_C_END

#endif /* CSEM_MICRO_TREE_H */

/**
 * \example sample_micro_tree.c
 * \example sample_micro_search.c
 */
