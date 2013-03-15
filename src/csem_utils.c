/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "csem_utils.h"

char *CSEM_Utils_Strcpy(const char *src) {
    return src ? CSEM_Utils_Strncpy(src, strlen(src)) : NULL;
}
char *CSEM_Utils_Strncpy(const char *src, size_t srcLen) {
    char *dest = NULL;
    if(!src) {
        return NULL;
    }
    if(!(dest = CSEM_Calloc(srcLen + 1, sizeof(char)))) {
        return NULL;
    }
    strncpy(dest, src, srcLen);
    return dest;
}
CSEM_Bool CSEM_Utils_Strcat(char *to, size_t to_size, int from_number, ...) {
    int i = 0;
    char *from;
    CSEM_Bool isSuccess = CSEM_TRUE;

    va_list ap;
    va_start(ap, from_number);

    REP(i, from_number) {
        from = va_arg(ap, char *);

        if(strlen(to) + strlen(from) < to_size) {
            strcat(to, from);
        } else {
            isSuccess = CSEM_FALSE;
            break;
        }
    }
    va_end(ap);
    return isSuccess;
}
CSEM_Error CSEM_Utils_Strtoks(char *src, const char *delim, CSEM_List **tokens) {
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
CSEM_Error CSEM_Utils_Strntoks(char *src, size_t srcLen, const char *delim, CSEM_List **tokens) {
    CSEM_Error error = CSEM_ERROR_NONE;
    char buf[CSEM_DEFAULT_BUF_SIZE];

    if(srcLen > sizeof(buf) - 1) {
        error = CSEM_ERROR_TOO_LARGE_FIELD;
        goto ERROR;
    }
    memset(buf, 0, sizeof(buf));
    strncpy(buf, src, srcLen);
    if((error = CSEM_Utils_Strtoks(buf, delim, tokens))) {
        goto ERROR;
    }
    return error;
ERROR:
    return error;
}
CSEM_Bool  CSEM_Utils_Strcmp(const char *s1, const char *s2) {
    return s1 && s2 ? !strcmp(s1, s2) : !s1 && !s2;
}
int CSEM_Utils_Strhash(const char *s) {
    int h = 0, len = 0;
    if(s && (len = strlen(s))) {
        int i = 0, offset = 0;
        for(i = 0; i < len; i++) {
            h = 31 * h + s[offset++];
        }
    }
    return h;
}
void CSEM_Utils_Unquote(char *in, size_t inLen, char **out) {
    *out = NULL;

    if(inLen >= 2) {
        if(in[0] == '"' && in[inLen -1] == '"') {
            in[inLen -1] = 0;
            in++;
        } else if(in[0] == '\'' && in[inLen -1] == '\'') {
            in[inLen -1] = 0;
            in++;
        }
    }
    *out = in;
}
void CSEM_Utils_ToLower(char *in) {
    while(*in) {
        *in = tolower(*in);
        in++;
    }
}
CSEM_Error CSEM_Utils_ToString(unsigned char *bin, size_t binLen, char **string) {
    size_t i = 0;
    if(!(*string = CSEM_Malloc(binLen * 2 + 1))) {
        return CSEM_ERROR_MEMORY;
    }
    REP(i, binLen) {
        sprintf((*string) + i * 2, "%02x", bin[i]);
    }
    (*string)[binLen * 2] = '\0';
    return CSEM_ERROR_NONE;
}
CSEM_Bool CSEM_Utils_IsLittleEndian() {
    int i = 1;
    return (*(char *)&i);
}
