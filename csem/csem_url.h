/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
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

/** URL data model. */
typedef struct CSEM_Url {
    char *scheme;
    char *hostname;
    char *port;
    char *path;
    char *query;
    char *fragment;
} CSEM_Url;

/**
 * Create url instance.
 */
CSEM_Url *CSEM_URL_Create();
/**
 * Dispose the url.
 * @param url [in]url to be freed
 */
void CSEM_URL_Dispose(CSEM_Url *url);
/**
 * Parse a url string.
 * @param urlstring [in]url string
 * @param url       [out]created url instance
 * @return error code
 */
CSEM_Error CSEM_URL_Parse(const char *urlstring, CSEM_Url **url);
/**
 * Serialize a url as string format.
 * @param url       [in]url
 * @param urlstring [out]string representation of the url
 * @return error code
 */
CSEM_Error CSEM_URL_Serialize(const CSEM_Url *url, char **urlstring);
/**
 * Merge a base url and a relative url.
 * @param base     [in]base url
 * @param relative [in]relative url
 * @param merged   [out]merged url
 * @return error code
 */
CSEM_Error CSEM_URL_Merge(const CSEM_Url *base, const CSEM_Url *relative, CSEM_Url **merged);
/**
 * Copy the specified url.
 * @param src  [in]the source url
 * @param dist [out]a newly created url
 * @return error code
 */
CSEM_Error CSEM_URL_Copy(const CSEM_Url *src, CSEM_Url **dist);
/**
 * Compare the specified URLs.
 * @param u1 [in]url
 * @param u2 [in]url
 * @return result
 */
CSEM_Bool CSEM_URL_Equal(const CSEM_Url *u1, const CSEM_Url *u2);
/*
 * Get hash code of the URL.
 * @param url  [in]url
 * @param hash [out]hash code
 * @return error code
 */
CSEM_Error CSEM_URL_Hash(const CSEM_Url *url, int *hash);

CSEM_NS_C_END

#endif /* CSEM_URL_H */
