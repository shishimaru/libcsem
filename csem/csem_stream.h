/* $Id$ */
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

typedef struct CSEM_Parser CSEM_Parser;
typedef struct CSEM_Handler CSEM_Handler;
typedef void (*CSEM_Parser_ErrorHandler)(const void *userdata, CSEM_Error error);

CSEM_Error CSEM_Handler_Create(CSEM_Handler **hanlder);
void       CSEM_Handler_Dispose(CSEM_Handler *handler, CSEM_Bool free);
void       CSEM_Handler_SetErrorHandler(CSEM_Handler *handler, CSEM_Parser_ErrorHandler error);
void       CSEM_Handler_SetMicrodataHandler(CSEM_Handler *handler, CSEM_Micro_Handlers *microdata);
void       CSEM_Handler_SetRDFaLiteHandler(CSEM_Handler *handler, CSEM_RDFaLite_Handlers *rdfa_lite);

CSEM_Error CSEM_Parser_Create(CSEM_Parser **parser);
void       CSEM_Parser_SetUserdata(CSEM_Parser *parser, void *userdata);
void       CSEM_Parser_SetErrorHandler(CSEM_Parser *parser, CSEM_Parser_ErrorHandler *handler);
void       CSEM_Parser_SetHandler(CSEM_Parser *parser, CSEM_Handler *handler);
CSEM_Error CSEM_Parser_Parse(CSEM_Parser *parser, int fd);
CSEM_Error CSEM_Parser_ParseChunk(CSEM_Parser *parser, const char *chunk, int size, int terminate);
void       CSEM_Parser_Dispose(CSEM_Parser *parser);

CSEM_NS_C_END

#endif /* CSEM_PARSER_H */

/**
 * \example sample_micro_stream.c
 */
