/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
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

#include "csem_tree.h"

/**
 * Data model builder.
 */
typedef struct CSEM_Builder CSEM_Builder;
/**
 * Create new builder.
 * @param builder [out]builder to be created
 * @return error code
 */
CSEM_Error CSEM_Builder_Create(CSEM_Builder **builder);
/**
 * Parse the specified input stream and build a data model.
 * @param builder [in]builder
 * @param fd      [in]input stream
 * @return error code
 */
CSEM_Error CSEM_Builder_Parse(CSEM_Builder *builder, int fd);
/**
 * Parse the specified chunk data and build a part of the data model.
 * @param builder   [in]builder
 * @param chunk     [in]chunk data
 * @param size      [in]size of the chunk
 * @param terminate [in]set 1 if the chunk is final data to be parsed. Otherwise, set 0.
 * @return error code
 */
CSEM_Error CSEM_Builder_ParseChunk(CSEM_Builder *builder,
        const char *chunk, int size, int terminate);
/**
 * Get the parsed result document.
 * @param builder [in]builder
 * @param doc     [out]parsed result document
 */
CSEM_Error CSEM_Builder_GetDocument(CSEM_Builder *builder, CSEM_Document **doc);
/**
 * Dispose the builder.
 * @param builder [in]builder to be freed
 */
void CSEM_Builder_Dispose(CSEM_Builder *builder);

CSEM_NS_C_END

#endif /* CSEM_PARSER_H */
