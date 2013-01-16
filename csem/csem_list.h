/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_LIST_H
#define CSEM_LIST_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include <stddef.h>
#include "csem_common.h"

/** Array List. */
typedef struct _CSEM_LIST CSEM_List;

/**
 * Create a list instance.
 * @param initial_size [in]a initial size of the array list
 * @return a newly created array list
 */
CSEM_List *CSEM_List_Create(size_t initial_size);

/**
 * Add a specified element into the list.
 * @param list [in]list instance
 * @param data [in]a element to be added
 * @return error code
 */
CSEM_Error CSEM_List_Add(CSEM_List *list, const void *data);

/**
 * Get a length of the list array.
 * @param list [in]list instance
 * @return length
 */
size_t CSEM_List_Size(const CSEM_List *list);

/**
 * Get an element of the specified index.
 * @param list  [in]list instance
 * @param index [in]index of the element
 * @return the specified element
 */
void *CSEM_List_Get(const CSEM_List *list, int index);

/**
 * Test whether the list contains the specified element or not.
 * @param list [in]list instance
 * @param data [in]an element to be tested
 * @return #CSEM_TRUE if contains
 */
CSEM_Bool CSEM_List_Contains(const CSEM_List *list, const void *data);

/**
 * Get an index of the specified element.
 * @param list [in]list instance
 * @param data [in]an element
 * @return the index
 */
int CSEM_List_IndexOf(const CSEM_List *list, const void *data);

/**
 * Test whether the list is empty or not.
 * @param list [in]list instance
 * @return #CSEM_TRUE if the list doesn't have any elements
 */
CSEM_Bool CSEM_List_IsEmpty(const CSEM_List *list);

/**
 * Remove an element of the specified index.
 * @param list  [in]list instance
 * @param index [in]index of the element to be removed
 * @return the removed element
 */
void *CSEM_List_Remove(CSEM_List *list, int index);

/**
 * Replace the specified index element with the specified new one.
 * @param list  [in]list instance
 * @param index [in]index of element to be replaced
 * @param data  [in]an new element
 * @return the replaced old element
 */
void *CSEM_List_Replace(CSEM_List *list, int index, const void *data);

/**
 * Clear all elements of the list.
 * @param list [in]list instance
 * @param free [in]a flag to free the elements or not
 */
void CSEM_List_Clear(CSEM_List *list, CSEM_Bool free);

/**
 * Dispose the list instance.
 * @param list [in]list instance to be freed
 * @param free [in]a flag to free the elements or not
 */
void CSEM_List_Dispose(CSEM_List *list, CSEM_Bool free);

CSEM_NS_C_END

#endif /* end of CSEM_LIST_H */
