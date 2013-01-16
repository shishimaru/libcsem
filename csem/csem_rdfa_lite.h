/* $Id$ */
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
 * @todo
 */
typedef struct CSEM_RDFaLite_Handlers CSEM_RDFaLite_Handlers;
typedef void (*CSEM_RDFaLite_Handler_StartScope)(const void *userdata, const CSEM_List *types, const CSEM_List *itemRefs);
typedef void (*CSEM_RDFaLite_Handler_ItemProp)(const void *userdata, const char *prop, const CSEM_List *values);
typedef void (*CSEM_RDFaLite_Handler_EndScope)(const void *userdata, const CSEM_List *types, const CSEM_List *itemRefs);

CSEM_Error CSEM_RDFaLite_CreateHandler(CSEM_RDFaLite_Handlers **handler);
void CSEM_RDFaLite_DisposeHandler(CSEM_RDFaLite_Handlers *handler);

CSEM_NS_C_END

#endif /* CSEM_RDFaLite_H */
