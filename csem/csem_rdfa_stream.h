/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_RDFaLite_H
#define CSEM_RDFaLite_H

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
 * @todo : not implemented
 */
typedef struct CSEM_RDFaLite_Handlers CSEM_RDFaLite_Handlers;
/**
 * Subject :
 *   if(!property && typeof)
 */
typedef CSEM_Bool (*CSEM_RDFaLite_StartScope)(const void *userdata, const CSEM_List *types, const char *resource);
typedef void (*CSEM_RDFaLite_EndScope)(const void *userdata);

typedef CSEM_Bool (*CSEM_RDFaLite_StartProp)(const void *userdata, const char *propName, CSEM_Bool hasUrlValue);
typedef void (*CSEM_RDFaLite_ItemProp)(const void *userdata, const char *value, int len);
typedef void (*CSEM_RDFaLite_EndProp)(const void *userdata);

CSEM_Error CSEM_RDFaLite_CreateHandler(CSEM_RDFaLite_Handlers **handler);
void CSEM_RDFaLite_DisposeHandler(CSEM_RDFaLite_Handlers *handler);
void CSEM_RDFaLite_SetStartScope(CSEM_RDFaLite_Handlers *handler, CSEM_RDFaLite_StartScope startScope);
void CSEM_RDFaLite_SetEndScope(CSEM_RDFaLite_Handlers *handler, CSEM_RDFaLite_EndScope endScope);
void CSEM_RDFaLite_SetStartItemProp(CSEM_RDFaLite_Handlers *handler, CSEM_RDFaLite_StartProp startProp);
void CSEM_RDFaLite_SetItemProp(CSEM_RDFaLite_Handlers *handler, CSEM_RDFaLite_ItemProp itemProp);
void CSEM_RDFaLite_SetEndItemProp(CSEM_RDFaLite_Handlers *handler, CSEM_RDFaLite_EndProp endProp);

CSEM_NS_C_END

#endif /* CSEM_RDFaLite_H */
