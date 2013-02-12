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
 * Handlers for HTML5 microdata streaming parser.
 */
typedef struct CSEM_Micro_Handlers CSEM_Micro_Handlers;
/**
 * Handler for start scope of an item.
 * @param userdata [out]user data
 * @param types    [out]types of the item
 * @param refs     [out]references of the item
 * @param id       [out]item id
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep it for further
 * processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_ItemStart)(const void *userdata,
        const CSEM_List *types, const CSEM_List *refs, const char *id);
/**
 * Handler for end scope of an item.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_ItemEnd)(const void *userdata);
/**
 * Handler for start scope of a property.
 * @param userdata [out]user data
 * @param propName [out]the property name TODO : support space separated names
 * @param hasUrlValue [out]#CSEM_TRUE if the value type of the property is URL.
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep the passed it for further
 * processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_PropStart)(const void *userdata, const char *propName, CSEM_Bool hasUrlValue);
/**
 * Handler for property value. The values may be divided into pieces
 * with multiple call back.
 * @param userdata [out]user data
 * @param value    [out]property value
 * @param len      [out]length of the value
 */
typedef void (*CSEM_Micro_PropValue)(const void *userdata, const char *value, int len);
/**
 * Handler for end scope of a property.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_PropEnd)(const void *userdata);
/**
 * Handler for start scope of an id referenced by \@itemref.
 * @param userdata [out]user data
 * @param id       [out]id
 * @return #CSEM_TRUE if the passed microdata on this event should be
 * freed by library. If applications would like to keep the passed
 * microdata for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_Micro_IdStart)(const void *userdata, const char *id);
/**
 * Handler for end scope of an id.
 * @param userdata [out]user data
 */
typedef void (*CSEM_Micro_IdEnd)(const void *userdata);
/**
 * Create a handler manager for microdata.
 * @param handler [out]created handler manager
 * @return error code
 */
CSEM_Error CSEM_Micro_CreateHandler(CSEM_Micro_Handlers **handler);
/**
 * Dispose a handler manager.
 * @param handler [in]handler manager to be freed
 */
void CSEM_Micro_DisposeHandler(CSEM_Micro_Handlers *handler);
/**
 * Set item start handler to the handler manager.
 * @param handler    [in]handler manager
 * @param startScope [in]item start handler
 */
void CSEM_Micro_SetItemStart(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemStart startScope);
/**
 * Set item end handler to the handler manager.
 * @param handler  [in]handler manager
 * @param endScope [in]item end handler
 */
void CSEM_Micro_SetItemEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemEnd endScope);
/**
 * Set property start handler to the handler manager.
 * @param handler   [in]handler manager
 * @param startProp [in]property start handler
 */
void CSEM_Micro_SetPropStart(CSEM_Micro_Handlers *handler, CSEM_Micro_PropStart startProp);
/**
 * Set property value handler to the handler manager.
 * @param handler   [in]handler manager
 * @param propValue [in]property value handler
 */
void CSEM_Micro_SetPropValue(CSEM_Micro_Handlers *handler, CSEM_Micro_PropValue propValue);
/**
 * Set property end handler to the handler manager.
 * @param handler [in]handler manager
 * @param endProp [in]property end handler
 */
void CSEM_Micro_SetPropEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_PropEnd endProp);
/**
 * Set id start handler to the handler manager.
 * @param handler [in]handler manager
 * @param startId [in]id start handler
 */
void CSEM_Micro_SetIdStart(CSEM_Micro_Handlers *handler, CSEM_Micro_IdStart startId);
/**
 * Set id end handler to the handler manager.
 * @param handler [in]handler manager
 * @param endId   [in]id end handler
 */
void CSEM_Micro_SetIdEnd(CSEM_Micro_Handlers *handler, CSEM_Micro_IdEnd endId);

CSEM_NS_C_END

#endif /* CSEM_MICRO_STREAM_H */
