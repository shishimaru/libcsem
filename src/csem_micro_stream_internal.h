/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_MICRODATA_INTERNAL_H
#define CSEM_MICRODATA_INTERNAL_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem/csem_micro_stream.h"

extern const char *HTML5_SPACES;
char *CSEM_Micro_GetAttNameWithPropValue(const char *localname, const char *ns);
CSEM_Bool CSEM_Micro_IsUrlPropElement(const char *localname, const char *ns);

CSEM_NS_C_END

#endif /* CSEM_MICRODATA_INTERNAL_H */
