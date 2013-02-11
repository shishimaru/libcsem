/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_NS_MANAGER_H
#define CSEM_NS_MANAGER_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem/csem_common.h"

typedef struct CSEM_NSManager CSEM_NSManager;

CSEM_NSManager *CSEM_NSManager_Create();
void CSEM_NSManager_Dispose(CSEM_NSManager *manager, CSEM_Bool free);
CSEM_Error CSEM_NSManager_Add(CSEM_NSManager *manager, const char *prefix, const char *uri);
CSEM_Error CSEM_NSManager_Push(CSEM_NSManager *manager);
const char *CSEM_NSManager_lookupURI(CSEM_NSManager *manager, const char *prefix);
void CSEM_NSManager_Pop(CSEM_NSManager *manager, CSEM_Bool free);

CSEM_NS_C_END

#endif /* CSEM_NS_MANAGER_H */
