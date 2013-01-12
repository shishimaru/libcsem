/* $Id$ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include <openssl/sha.h>
#include <openssl/md5.h>

#include "csem_utils.h"

char *CSEM_Utils_Strcpy(const char *src) {
    if(!src) {
        return NULL;
    }
    return CSEM_Utils_Strncpy(src, strlen(src));
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

    for(i = 0; i < from_number; i++) {
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
        CSEM_List_Add(result, value);
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
    for(i = 0; i < binLen; i++) {
        sprintf((*string) + i * 2, "%02x", bin[i]);
    }
    (*string)[binLen * 2] = '\0';
    return CSEM_ERROR_NONE;
}
static void csem_utils_convertEndian(size_t size, void *in, void *out) {
    int i = 0;
    for(i = 0; i < size; i++) {
        ((char *)out)[i] = ((char *)in)[size-1-i];
    }
}
CSEM_UINT64_T CSEM_Utils_ConvertEndianUINT64(CSEM_UINT64_T in) {
    CSEM_UINT64_T out = 0;
    csem_utils_convertEndian(sizeof(CSEM_UINT64_T), &in, &out);
    return out;
}
CSEM_UINT32_T CSEM_Utils_ConvertEndianUINT32(CSEM_UINT32_T in) {
    CSEM_UINT32_T out = 0;
    csem_utils_convertEndian(sizeof(CSEM_UINT32_T), &in, &out);
    return out;
}
CSEM_UINT16_T CSEM_Utils_ConvertEndianUINT16(CSEM_UINT16_T in) {
    CSEM_UINT16_T out = 0;
    csem_utils_convertEndian(sizeof(CSEM_UINT16_T), &in, &out);
    return out;
}
CSEM_Bool CSEM_Utils_IsLittleEndian() {
    int i = 1;
    return (*(char *)&i);
}
CSEM_UINT16_T CSEM_Utils_htobe16(CSEM_UINT16_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT16(input) :
            input;
}
CSEM_UINT32_T CSEM_Utils_htobe32(CSEM_UINT32_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT32(input) :
            input;
}
CSEM_UINT64_T CSEM_Utils_htobe64(CSEM_UINT64_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT64(input) :
            input;
}
CSEM_UINT16_T CSEM_Utils_be16toh(CSEM_UINT16_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT16(input) :
            input;
}
CSEM_UINT32_T CSEM_Utils_be32toh(CSEM_UINT32_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT32(input) :
            input;
}
CSEM_UINT64_T CSEM_Utils_be64toh(CSEM_UINT64_T input) {
    return CSEM_Utils_IsLittleEndian() ?
            CSEM_Utils_ConvertEndianUINT64(input) :
            input;
}
