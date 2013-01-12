/* $Id$ */
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

#include "csem_common.h"
#include "csem_list.h"

typedef struct CSEM_Micro_Handlers CSEM_Micro_Handlers;

typedef CSEM_Bool (*CSEM_Micro_StartScope)(const void *userdata, const CSEM_List *types, const CSEM_List *refs, const char *id);
typedef void (*CSEM_Micro_EndScope)(const void *userdata);
typedef CSEM_Bool (*CSEM_Micro_StartProp)(const void *userdata, const char *propName, CSEM_Bool hasUrlValue);
typedef void (*CSEM_Micro_ItemProp)(const void *userdata, const char *value, int len);
typedef void (*CSEM_Micro_EndProp)(const void *userdata);
typedef CSEM_Bool (*CSEM_Micro_StartId)(const void *userdata, const char *id);
typedef void (*CSEM_Micro_EndId)(const void *userdata);

CSEM_Error CSEM_Micro_CreateHandler(CSEM_Micro_Handlers **handler);
void       CSEM_Micro_DisposeHandler(CSEM_Micro_Handlers *handler);
void       CSEM_Micro_SetStartScope(CSEM_Micro_Handlers *handler, CSEM_Micro_StartScope startScope);
void       CSEM_Micro_SetEndScope(CSEM_Micro_Handlers *handler, CSEM_Micro_EndScope endScope);
void       CSEM_Micro_SetStartItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_StartProp startProp);
void       CSEM_Micro_SetItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_ItemProp propValue);
void       CSEM_Micro_SetEndItemProp(CSEM_Micro_Handlers *handler, CSEM_Micro_EndProp endProp);
void       CSEM_Micro_SetStartId(CSEM_Micro_Handlers *handler, CSEM_Micro_StartId startId);
void       CSEM_Micro_SetEndId(CSEM_Micro_Handlers *handler, CSEM_Micro_EndId endId);

CSEM_NS_C_END

#endif /* CSEM_MICRO_STREAM_H */
