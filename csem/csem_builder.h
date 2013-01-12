/* $Id$ */
#ifndef CSEM_BUILDER_H
#define CSEM_BUILDER_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem_common.h"
#include "csem_tree.h"

typedef struct CSEM_Builder CSEM_Builder;

CSEM_Error CSEM_Builder_Create(CSEM_Builder **builder);
CSEM_Error CSEM_Builder_Parse(CSEM_Builder *builder, int fd, CSEM_Document **doc, CSEM_Bool simplify);
void       CSEM_Builder_Dispose(CSEM_Builder *builder);

CSEM_NS_C_END

#endif /* CSEM_PARSER_H */
