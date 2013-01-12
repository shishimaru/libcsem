/* $Id$ */
#ifndef CSEM_URL_H
#define CSEM_URL_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include "csem_common.h"

/** URL data model */
typedef struct CSEM_Url {
    char *scheme;
    char *hostname;
    char *port;
    char *path;
    /** query part excluding '?' */
    char *query;
    /** fragment part excluding '#' */
    char *fragment;
} CSEM_Url;

/**
 * Parse a RFC3986 url string and create a url instance.
 * @param urlstring [in]url string to be parsed
 * @param url       [out]created url instance
 * @return error code related to url parsing
 */
CSEM_Error CSEM_URL_Parse(const char *urlstring, CSEM_Url **url);
/**
 * Merge a base url and a relative url based on a relative url resolution of RFC3986.
 * @param base     [in]a base url
 * @param relative [in]a relative url
 * @param merged   [out]the merged url
 * @return error code related to memory allocation
 */
CSEM_Error CSEM_URL_Merge(const CSEM_Url *base, const CSEM_Url *relative, CSEM_Url **merged);
/**
 * Create empty url instance.
 */
CSEM_Url *CSEM_URL_Create();
/**
 * Copy the specified url.
 * @param src  [in]the source url
 * @param dist [out]a newly created url
 * @return error code related to memory allocation
 */
CSEM_Error CSEM_URL_Copy(const CSEM_Url *src, CSEM_Url **dist);
/**
 * Serialize the url to string representation.
 * @param url       [in]url
 * @param urlstring [out]string representation of the url
 * @return error code related to url serializing
 */
CSEM_Error CSEM_URL_Serialize(const CSEM_Url *url, char **urlstring);
/**
 * Dispose the specified url instance.
 * @param url [in]url instance to be freed
 */
void CSEM_URL_Dispose(CSEM_Url *url);

CSEM_NS_C_END

#endif /* CSEM_URL_H */
