/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
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

#include "csem_tree.h"

/**
 * Item object defined with \@itemscope.
 */
typedef struct CSEM_Item CSEM_Item;
/**
 * Id object defined with \@id.
 */
typedef struct CSEM_Id CSEM_Id;
/**
 * Property object defined with \@itemprop.
 */
typedef struct CSEM_Property CSEM_Property;
/**
 * Types of property value.
 */
typedef enum {
    /** String */
    CSEM_MICRO_VALUE_TYPE_STR = 0,
    /** URL */
    CSEM_MICRO_VALUE_TYPE_URL = 1,
    /** Item */
    CSEM_MICRO_VALUE_TYPE_ITEM = 2,
    /** Property */
    CSEM_MICRO_VALUE_TYPE_PROPERTY = 3
} CSEM_MICRO_VALUE_TYPE;
/**
 * Create an item object.
 * @param item [out]item object to be created
 * @return error code
 */
CSEM_Error CSEM_Micro_Item_Create(CSEM_Item **item);
/**
 * Dispose an item object.
 * @param item [in]item object to be freed
 */
void CSEM_Micro_Item_Dispose(CSEM_Item *item);
/**
 * Get \@itemid value of the item.
 * @param item [in]item
 */
char *CSEM_Micro_Item_GetId(CSEM_Item *item);
/**
 * Set \@itemid value.
 * @param item [in]item
 * @param id   [in]\@itemid value (not copied internally)
 * @param free [in]if #CSEM_TRUE is set, the old value is freed.
 * @return error code
 */
CSEM_Error CSEM_Micro_Item_SetId(CSEM_Item *item, const char *id, CSEM_Bool free);
/**
 * Get \@itemtype values.
 * @param item [in]item
 */
CSEM_List *CSEM_Micro_Item_GetTypes(const CSEM_Item *item);
/**
 * Set \@itemtype values.
 * @param item  [in]item
 * @param types [in]\@itemtype values (not copied internally)
 * @param free  [in]if #CSEM_TRUE is set, the old \@itemtype values are freed.
 */
CSEM_Error CSEM_Micro_Item_SetTypes(CSEM_Item *item, const CSEM_List *types, CSEM_Bool free);
/**
 * Get \@itemref values.
 * @param item [in]item
 */
CSEM_List *CSEM_Micro_Item_GetRefs(CSEM_Item *item);
/**
 * Set \@itemref values.
 * @param item [in]item
 * @param refs [in]\@itemref values (not copied internally)
 * @param free [in]if #CSEM_TRUE is set, the old \@itemref values are freed.
 */
CSEM_Error CSEM_Micro_Item_SetRefs(CSEM_Item *item, const CSEM_List *refs, CSEM_Bool free);
/**
 * Get properties of the item.
 * @param item [in]item
 */
CSEM_List *CSEM_Micro_Item_GetProperties(const CSEM_Item *item);
/**
 * Get properties whose name is equal to the specified name.
 * @param item       [in]item
 * @param name       [in]name to be tested
 * @param properties [out]matched properties (applications need to free with CSEM_List_Dispose(properties, CSEM_FALSE))
 * @return error code
 */
CSEM_Error CSEM_Micro_Item_GetNamedPropertes(const CSEM_Item *item, const char *name, CSEM_List **properties);
/**
 * Add a property to the item.
 * @param item         [in]item
 * @param property     [in]property to be added
 * @param changeParent [in]flag whether the property keeps current relationship with parent
 * @return error code
 */
CSEM_Error CSEM_Micro_Item_AddProperty(CSEM_Item *item, CSEM_Property *property, CSEM_Bool changeParent);
/**
 * Create id object with \@id value
 * @param id    [out]id object to be created
 * @param value [in]id value (not copied internally)
 * @return error code
 */
CSEM_Error CSEM_Micro_Id_Create(CSEM_Id **id, const char *value);
/**
 * Dispose id object.
 * @param id [in]id object to be freed
 */
void CSEM_Micro_Id_Dispose(CSEM_Id *id);
/**
 * Get properties of the id object.
 * @param id [in]id object
 */
CSEM_List *CSEM_Micro_Id_GetProperties(CSEM_Id *id);
/**
 * Add a property to the id object.
 * @param id       [in]id object
 * @param property [in]property to be added
 * @return error code
 */
CSEM_Error CSEM_Micro_Id_AddProperty(CSEM_Id *id, CSEM_Property *property);
/**
 * Get \@id value of the id object.
 * @param id [in]id object
 */
char *CSEM_Micro_Id_GetId(CSEM_Id *id);
/**
 * Create a property object which has the specified property names.
 * @param property [out]property object to be created
 * @param names    [in]\@itemprop value (not copied internally)
 * @return error code
 */
CSEM_Error CSEM_Micro_Property_Create(CSEM_Property **property, CSEM_List *names);
/**
 * Dispose a property object.
 * @param property [in]property object to be freed
 */
void CSEM_Micro_Property_Dispose(CSEM_Property *property);
/**
 * Get names of the property.
 * @param property [in]property object
 */
CSEM_List *CSEM_Micro_Property_GetNames(CSEM_Property *property);
/**
 * Get values and the types of the values of the specified property. The type of values[n] is valueTypes[n].
 * @param property   [in]property
 * @param values     [out]values
 * @param valueTypes [out]types of the values
 * @return error code
 */
CSEM_Error CSEM_Micro_Property_GetValues(CSEM_Property *property, CSEM_List **values, CSEM_List **valueTypes);
/**
 * Add a value to the property.
 * @param property [in]property
 * @param value    [in]value
 * @param type     [in]type of the value
 * @return error code
 */
CSEM_Error CSEM_Micro_Property_AddValues(CSEM_Property *property, void *value, CSEM_MICRO_VALUE_TYPE type);
/**
 * Get values and the types of the values of the specified properties. The type of values[n] is valueTypes[n].
 * @param properties [in]properties
 * @param values     [out]values (applications need to free with CSEM_List_Dispose(values, CSEM_FALSE))
 * @param valueTypes [out]types of the values (applications need to free with CSEM_List_Dispose(valueTypes, CSEM_FALSE))
 * @return error code
 */
CSEM_Error CSEM_Micro_Properties_GetValues(CSEM_List *properties, CSEM_List **values, CSEM_List **valueTypes);
/**
 * Get items which have the specified \@itemtype.
 * @param doc   [in]document object
 * @param types [in]\@itemtype value
 * @param items [out]matched items (application need to free with CSEM_List_Dispose(items, CSEM_FALSE))
 * @return error code
 */
CSEM_Error CSEM_Micro_Document_GetItems(CSEM_Document *doc, CSEM_List *types, CSEM_List **items);

CSEM_NS_C_END

#endif /* CSEM_MICRO_TREE_H */

/**
 * \example sample_micro_tree.c
 * \example sample_micro_search.c
 */
