/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_PARSER_H
#define CSEM_PARSER_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem_micro_stream.h"
#include "csem_rdfa_lite.h"

/**
 * Streaming parser.
 */
typedef struct CSEM_Parser CSEM_Parser;
/**
 * Callback handler manager.
 * The streaming parser passes the parsed microdata
 * to applications via the registered handlers.
 */
typedef struct CSEM_Handler CSEM_Handler;
/**
 * Error handler. When the parser detects any error related to parser processing,
 * the corresponding error code is passed.
 * @param userdata [out]user data
 * @param error    [out]error code
 */
typedef void (*CSEM_Parser_ErrorHandler)(const void *userdata, CSEM_Error error);
/**
 * Create a handler manager.
 * @param handler [out]created handler manager
 * @return error code
 */
CSEM_Error CSEM_Handler_Create(CSEM_Handler **handler);
/**
 * Dispose a handler manager.
 * @param handler [in]handler manager to be freed
 * @param free    [in]if #CSEM_TRUE is set, the registered
 * underlying handlers are also freed at the same time.
 */
void CSEM_Handler_Dispose(CSEM_Handler *handler, CSEM_Bool free);
/**
 * Set error handler to the handler manager.
 * @param handler [in]handler manager
 * @param error   [in]error handler
 */
void CSEM_Handler_SetErrorHandler(CSEM_Handler *handler, CSEM_Parser_ErrorHandler error);
/**
 * Set microdata handler to the handler manager.
 * @param handler   [in]handler manager
 * @param microdata [in]microdata handler
 */
void CSEM_Handler_SetMicrodataHandler(CSEM_Handler *handler, CSEM_Micro_Handlers *microdata);
/**
 * Set RDFa-Lite handler to the handler manager.
 * @todo
 * @param handler   [in]manager
 * @param rdfa_lite [in]RDFa-Lite handler
 */
void CSEM_Handler_SetRDFaLiteHandler(CSEM_Handler *handler, CSEM_RDFaLite_Handlers *rdfa_lite);
/**
 * Create a streaming parser.
 * @param parser [in]created streaming parser
 * @return error code
 */
CSEM_Error CSEM_Parser_Create(CSEM_Parser **parser);
/**
 * Set a user data.
 * @param parser   [in]streaming parser
 * @param userdata [in]user data to be registered
 */
void CSEM_Parser_SetUserdata(CSEM_Parser *parser, void *userdata);
/**
 * Set a handler manager to the parser.
 * @param parser  [in]streaming parser
 * @param handler [in]handler manager
 */
void CSEM_Parser_SetHandler(CSEM_Parser *parser, CSEM_Handler *handler);
/**
 * Start the parsing process.
 * @param parser [in]streaming parser
 * @param fd     [in]any input stream to be parsed
 * @return error code
 */
CSEM_Error CSEM_Parser_Parse(CSEM_Parser *parser, int fd);
/**
 * Set a chunk to be parsed.
 * @param parser    [in]streaming parser
 * @param chunk     [in]chunk data
 * @param size      [in]size of the chunk
 * @param terminate [in]set 1 if the chunk is final data to be parsed. Otherwise, set 0.
 * @return error code
 */
CSEM_Error CSEM_Parser_ParseChunk(CSEM_Parser *parser, const char *chunk, int size, int terminate);
/**
 * Dispose the streaming parser.
 * @param parser [in]streming parser to be freed
 */
void CSEM_Parser_Dispose(CSEM_Parser *parser);

CSEM_NS_C_END

#endif /* CSEM_PARSER_H */

/**
 * \example sample_micro_stream.c
 */
