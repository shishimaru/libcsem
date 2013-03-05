/* $Id$ */
#include <stdio.h>
#include <fcntl.h>
#include "csem/csem_stream.h"

CSEM_Bool startScope(const void *userdata, const CSEM_List *types, const char *resource) {
    int i = 0;
    puts("START_SCOPE");

    if(resource) {
        printf("\t@resource=\"%s\"\n", resource);
    }
    for(i = 0; types && i < CSEM_List_Size(types); i++) {
        printf("\t@typeof=\"%s\"\n", (char *)CSEM_List_Get(types, i));
    }
    return CSEM_TRUE;
}
void endScope(const void *userdata) {
    puts("END_SCOPE\n");
}
CSEM_Bool startProp(const void *userdata, const char *propName, CSEM_Bool hasUrlValue) {
    puts("START_PROP");
    printf("\t@property=\"%s\"\n", propName);
    return CSEM_TRUE;
}
void itemProp(const void *userdata, const char *value, int len) {
    puts("PROP_VALUE");
    printf("\t");
    fwrite(value, 1, len, stdout);
    printf("\n");
}
void endProp(const void *userdata) {
    puts("END_PROP\n");
}

/**
 * Sample of light weight stream parser for RDFa Lite
 */
int main(int argc, char *argv[]) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Parser *stream = NULL;
    CSEM_Handler *handler = NULL;
    CSEM_RDFa_Handlers *rdfaHandler = NULL;
    char *filename = NULL;
    int fd = -1;

    /* setup input parameters */
    if(argc < 2) {
        error = CSEM_ERROR_PARAMETER;
        printf("./sample [filename]\n");
        goto FINISH;
    }
    filename = argv[1];

    /* init */
    if((error = CSEM_Parser_Create(&stream))) {
        fprintf(stderr, "failed to create parser\n");
        goto FINISH;
    }
    if((error = CSEM_Handler_Create(&handler))) {
        fprintf(stderr, "failed to create handler\n");
        goto FINISH;
    }
    if((error = CSEM_RDFa_CreateHandler(&rdfaHandler))) {
        fprintf(stderr, "failed to create RDFa Lite handler\n");
        goto FINISH;
    }
    /* register callback handlers to stream parser */
    CSEM_RDFa_SetItemStart(rdfaHandler, startScope);
    CSEM_RDFa_SetItemEnd(rdfaHandler, endScope);
    CSEM_RDFa_SetPropStart(rdfaHandler, startProp);
    CSEM_RDFa_SetPropValue(rdfaHandler, itemProp);
    CSEM_RDFa_SetPropEnd(rdfaHandler, endProp);
    CSEM_Handler_SetRDFaHandler(handler, rdfaHandler);
    CSEM_Parser_SetHandler(stream, handler);

    /* start parse process */
    if(!(fd = open(filename, O_RDONLY))) {
        fprintf(stderr, "failed to open %s\n", filename);
        goto FINISH;
    }
    if((error = CSEM_Parser_Parse(stream, fd))) {
        fprintf(stderr, "failed parsing %s\n", filename);
        goto FINISH;
    }
FINISH:
    CSEM_Parser_Dispose(stream);
    CSEM_Handler_Dispose(handler, CSEM_TRUE);
    puts("END");
    return error ? 1 : 0;
}
