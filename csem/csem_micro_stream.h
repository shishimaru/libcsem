/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_MICRO_STREAM_H
#define CSEM_MICRO_STREAM_H

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
 * Handlers for HTML5 microdata.
 */
typedef struct CSEM_Micro_Handlers CSEM_Micro_Handlers;
/**
 * Handler for start scope of an item.
 * @param userdata [out]user data
 * @param types    [out]types of the item
 * @param refs     [out]references of the item
 * @param id       [out]item id
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep the passed
 * microdata for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_StartScope)(const void *userdata, const CSEM_List *types, const CSEM_List *refs, const char *id);
/**
 * Handler for end scope of an item.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_EndScope)(const void *userdata);
/**
 * Handler for start property.
 * @param userdata [out]user data
 * @param propName [out]the property name TODO : support space separated names
 * @param hasUrlValue [out]#CSEM_TRUE if the value type of the property is URL.
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep the passed
 * microdata for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_StartProp)(const void *userdata, const char *propName, CSEM_Bool hasUrlValue);
/**
 * Handler for property value. If the length of the value is long,
 * the values are divided and this handler is called back more than once.
 * @param userdata [out]user data
 * @param value    [out]the value
 * @param len      [out]the length of the value
 */
typedef void (*CSEM_Micro_ItemProp)(const void *userdata, const char *value, int len);
/**
 * Handler for end property.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_EndProp)(const void *userdata);
/**
 * Handler for start id referenced by items with \@itemref.
 * @param userdata [out]user data
 * @param id       [out]id
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep the passed
 * microdata for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_StartId)(const void *userdata, const char *id);
/**
 * Handler for end id.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_EndId)(const void *userdata);
/**
 * Create a handler manager for microdata.
 * @param handler [in]created handler manager
 * @return error code
 */
CSEM_Error CSEM_Micro_CreateHandler(CSEM_Micro_Handlers **handler);
/**
 * Dispose a handler manager.
 * @param handler [in]handler manager to be freed
 */
void CSEM_Micro_DisposeHandler(CSEM_Micro_Handlers *handler);
/**
 * Set start cope handler to the handler manager.
 * @param handler    [in]handler manager
 * @param startScope [in]start cope handler
 */
void CSEM_Micro_SetStartScope(CSEM_Micro_Handlers *handler, CSEM_Micro_StartScope startScope);
/**
 * Set end cope handler to the handler manager.
 * @param handler  [in]handler manager
 * @param endScope [in]end cope handler
 */
void CSEM_Micro_SetEndScope(CSEM_Micro_Handlers *handler, CSEM_Micro_EndScope endScope);
/**
 * Set start property handler to the handler manager.
 * @param handler   [in]handler manager
 * @param startProp [in]start property handler
 */
void CSEM_Micro_SetStartItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_StartProp startProp);
/**
 * Set property value handler to the handler manager.
 * @param handler   [in]handler manager
 * @param propValue [in]property value handler
 */
void CSEM_Micro_SetItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemProp propValue);
/**
 * Set end property handler to the handler manager.
 * @param handler [in]handler manager
 * @param endProp [in]end property handler
 */
void CSEM_Micro_SetEndItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_EndProp endProp);
/**
 * Set start id handler to the handler manager.
 * @param handler [in]handler manager
 * @param startId [in]start id handler
 */
void CSEM_Micro_SetStartId(CSEM_Micro_Handlers *handler, CSEM_Micro_StartId startId);
/**
 * Set end id handler to the handler manager.
 * @param handler [in]handler manager
 * @param endId   [in]end id handler
 */
void CSEM_Micro_SetEndId(CSEM_Micro_Handlers *handler, CSEM_Micro_EndId endId);

CSEM_NS_C_END

#endif /* CSEM_MICRO_STREAM_H */
