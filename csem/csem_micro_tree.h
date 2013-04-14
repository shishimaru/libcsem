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

#include "csem_rdfa_tree.h"

/**
 * Id object defined with \@id.
 */
typedef struct CSEM_Id CSEM_Id;

/**
 * Get \@itemref values.
 * @param item [in]item
 */
CSEM_List *CSEM_Item_GetRefs(CSEM_Item *item);
/**
 * Set \@itemref values.
 * @param item [in]item
 * @param refs [in]\@itemref values (not copied internally)
 * @param free [in]if #CSEM_TRUE is set, the old \@itemref values are freed.
 */
CSEM_Error CSEM_Item_SetRefs(CSEM_Item *item, const CSEM_List *refs, CSEM_Bool free);
/**
 * Create id object with \@id value
 * @param id    [out]id object to be created
 * @param value [in]id value (not copied internally)
 * @return error code
 */
CSEM_Error CSEM_Id_Create(CSEM_Id **id, const char *value);
/**
 * Dispose id object.
 * @param id [in]id object to be freed
 */
void CSEM_Id_Dispose(CSEM_Id *id);
/**
 * Get properties of the id object.
 * @param id [in]id object
 */
CSEM_List *CSEM_Id_GetProperties(CSEM_Id *id);
/**
 * Add a property to the id object.
 * @param id       [in]id object
 * @param property [in]property to be added
 * @return error code
 */
/*CSEM_Error CSEM_Id_AddProperty(CSEM_Id *id, CSEM_Property *property);*/
CSEM_Error CSEM_Id_AddProperty(CSEM_Id *id, CSEM_Property *property, CSEM_Bool changeParent);
/**
 * Get \@id value of the id object.
 * @param id [in]id object
 */
char *CSEM_Id_GetId(CSEM_Id *id);

CSEM_NS_C_END

#endif /* CSEM_MICRO_TREE_H */

/**
 * \example sample_micro_tree.c
 * \example sample_micro_search.c
 */
