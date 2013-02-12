/* $Id$ */
#include <stdio.h>
#include <fcntl.h>
#include "csem/csem_stream.h"

CSEM_Bool startScope(const void *userdata, const CSEM_List *types,
        const CSEM_List *refs, const char *id) {
    int i = 0;
    puts("START_SCOPE");

    for(i = 0; types && i < CSEM_List_Size(types); i++) {
        printf("\t@itemtype=\"%s\"\n", (char *)CSEM_List_Get(types, i));
    }
    for(i = 0; refs && i < CSEM_List_Size(refs); i++) {
        printf("\t@itemref=\"%s\"\n", (char *)CSEM_List_Get(refs, i));
    }
    return CSEM_TRUE;
}
void endScope(const void *userdata) {
    puts("END_SCOPE\n");
}
CSEM_Bool startProp(const void *userdata, const char *propName, CSEM_Bool hasUrlValue) {
    puts("START_PROP");
    printf("\t@itemprop=\"%s\"\n", propName);
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
CSEM_Bool startId(const void *userdata, const char *id) {
    puts("START_ID");
    printf("\t@id=\"%s\"\n", id);
    return CSEM_TRUE;
}
void endId(const void *userdata) {
    puts("END_ID\n");
}

/**
 * Sample of light weight stream parser for HTML5 microdata.
 */
int main(int argc, char *argv[]) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Parser *stream = NULL;
    CSEM_Handler *handler = NULL;
    CSEM_Micro_Handlers *microHandler = NULL;
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
    if((error = CSEM_Micro_CreateHandler(&microHandler))) {
        fprintf(stderr, "failed to create micro handler\n");
        goto FINISH;
    }
    /* register callback handlers to stream parser */
    CSEM_Micro_SetItemStart(microHandler, startScope);
    CSEM_Micro_SetItemStart(microHandler, startScope);
    CSEM_Micro_SetItemEnd(microHandler, endScope);
    CSEM_Micro_SetPropStart(microHandler, startProp);
    CSEM_Micro_SetPropValue(microHandler, itemProp);
    CSEM_Micro_SetPropEnd(microHandler, endProp);
    CSEM_Micro_SetIdStart(microHandler, startId);
    CSEM_Micro_SetIdEnd(microHandler, endId);
    CSEM_Handler_SetMicrodataHandler(handler, microHandler);
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
