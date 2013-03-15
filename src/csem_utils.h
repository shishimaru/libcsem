/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_UTILS_H
#define CSEM_UTILS_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "stdlib.h"
#include "csem/csem_common.h"
#include "csem/csem_list.h"

#define CSEM_Malloc(size) malloc(size)
#define CSEM_Calloc(nmemb, size) calloc(nmemb, size)
#define CSEM_Realloc(ptr, size) realloc(ptr, size)
#define CSEM_Free(p) {if(p) free(p);}

#define CSEM_MAX(x, y) ((x) > (y) ? (x) : (y))
#define CSEM_MIN(x, y) ((x) < (y) ? (x) : (y))
#define FOR(i,a,b) for(i=(a);i<(b);++i)
#define REP(i,n) FOR(i,0,n)
#define CLR(a) memset((a), 0 ,sizeof(a))

char      *CSEM_Utils_Strcpy(const char *src);
char      *CSEM_Utils_Strncpy(const char *src, size_t srcLen);
CSEM_Bool  CSEM_Utils_Strcat(char *to, size_t to_size, int from_number, ...);
CSEM_Error CSEM_Utils_Strtoks(char *src, const char *delim, CSEM_List **tokens);
CSEM_Error CSEM_Utils_Strntoks(char *src, size_t srcLen, const char *delim, CSEM_List **tokens);
CSEM_Bool  CSEM_Utils_Strcmp(const char *s1, const char *s2);
int        CSEM_Utils_Strhash(const char *s);
void       CSEM_Utils_Unquote(char *in, size_t inLen, char **out);
void       CSEM_Utils_ToLower(char *in);
CSEM_Error CSEM_Utils_ToString(unsigned char *bin, size_t binLen, char **string);
CSEM_Bool  CSEM_Utils_IsLittleEndian();


CSEM_NS_C_END

#endif /* CSEM_UTILS_H */
