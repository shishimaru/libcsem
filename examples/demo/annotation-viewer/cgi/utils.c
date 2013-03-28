#include <stdio.h>
#include <string.h>
#include "utils.h"

int Utils_Strtoks(char *src, const char *delim, CSEM_List **tokens) {
    CSEM_Error error = CSEM_ERROR_NONE;
    char *token = NULL;
    char *value = NULL;
    CSEM_List *result = NULL;
    char *in = src;

    /* init */
    delim = delim ? delim : "";
    if(!(result = CSEM_List_Create(8))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }

    while((token = strtok(in, delim))) {
        if(!(value = CSEM_Utils_Strcpy(token))) {
            error = CSEM_ERROR_MEMORY;
            goto ERROR;
        }
        if((error = CSEM_List_Add(result, value))) {
            goto ERROR;
        }
        in = NULL;
    }

    /* result */
    *tokens = result;

    return error;
ERROR:
    CSEM_List_Dispose(result, CSEM_TRUE);
    return error;
}
int Utils_Find(CSEM_List *queries, const char *key, char **value) {
    int i = 0;
    *value = NULL;
    for(i = 0; i < CSEM_List_Size(queries); i++) {
        char *query = CSEM_List_Get(queries, i);
        if(query) {
            char *v = NULL;
            if((v = strchr(query, '='))) {
                *value = v + 1;
                break;
            }
        }
    }
    return 0;
}
