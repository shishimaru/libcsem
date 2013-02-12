/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_RDFa_STREAM_H
#define CSEM_RDFa_STREAM_H

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
 * Handlers for RDFa-Lite streaming parser.
 */
typedef struct CSEM_RDFa_Handlers CSEM_RDFa_Handlers;
/**
 * Handler for start scope of an item.
 * @param userdata [out]user data
 * @param types    [out]types of the item
 * @param resource [out]item id
 * @return #CSEM_TRUE if the passed RDFa on this event should be freed by library.
 * If applications would like to keep it for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_RDFa_ItemStart)(const void *userdata, const CSEM_List *types, const char *resource);
/**
 * Handler for end scope of an item.
 * @param userdata [out]user data
 */
typedef void (*CSEM_RDFa_ItemEnd)(const void *userdata);
/**
 * Handler for start scope of a property.
 * @param userdata [out]user data
 * @param propName [out]the property name TODO : support space separated names
 * @param hasUrlValue [out]#CSEM_TRUE if the value type of the property is URL.
 * @return #CSEM_TRUE if the passed RDFa on this event should be freed by library.
 * If applications would like to keep it for further processing, return #CSEM_FALSE.
 */
typedef CSEM_Bool (*CSEM_RDFa_PropStart)(const void *userdata, const char *propName, CSEM_Bool hasUrlValue);
/**
 * Handler for property value. The values may be divided into pieces
 * with multiple call back.
 * @param userdata [out]user data
 * @param value    [out]property value
 * @param len      [out]length of the value
 */
typedef void (*CSEM_RDFa_PropValue)(const void *userdata, const char *value, int len);
/**
 * Handler for end scope of a property.
 * @param userdata [out]user data
 */
typedef void (*CSEM_RDFa_PropEnd)(const void *userdata);
/**
 * Create a handler manager for RDFa Lite.
 * @param handler [out]created handler manager
 * @return error code
 */
CSEM_Error CSEM_RDFa_CreateHandler(CSEM_RDFa_Handlers **handler);
/**
 * Dispose a handler manager.
 * @param handler [in]handler manager to be freed
 */
void CSEM_RDFa_DisposeHandler(CSEM_RDFa_Handlers *handler);
/**
 * Set item start handler to the handler manager.
 * @param handler    [in]handler manager
 * @param itemStart [in]item start handler
 */
void CSEM_RDFa_SetItemStart(CSEM_RDFa_Handlers *handler, CSEM_RDFa_ItemStart itemStart);
/**
 * Set item end handler to the handler manager.
 * @param handler  [in]handler manager
 * @param itemEnd [in]item end handler
 */
void CSEM_RDFa_SetItemEnd(CSEM_RDFa_Handlers *handler, CSEM_RDFa_ItemEnd itemEnd);
/**
 * Set property start handler to the handler manager.
 * @param handler   [in]handler manager
 * @param propStart [in]property start handler
 */
void CSEM_RDFa_SetPropStart(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropStart propStart);
/**
 * Set property value handler to the handler manager.
 * @param handler   [in]handler manager
 * @param propValue [in]property value handler
 */
void CSEM_RDFa_SetPropValue(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropValue propValue);
/**
 * Set property end handler to the handler manager.
 * @param handler [in]handler manager
 * @param propEnd [in]property end handler
 */
void CSEM_RDFa_SetPropEnd(CSEM_RDFa_Handlers *handler, CSEM_RDFa_PropEnd propEnd);

CSEM_NS_C_END

#endif /* CSEM_RDFa_STREAM_H */
