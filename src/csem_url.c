/* $Id$ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csem/csem_url.h"
#include "csem_utils.h"

#define MAXLEN_SCHEME   16
#define MAXLEN_HOST     1024
#define MAXLEN_PORT     8
#define MAXLEN_PATH     1024
#define MAXLEN_QUERY    1024
#define MAXLEN_FRAGMENT 1024

typedef enum PARSE_STATE {
    PARSE_STATE_NONE,
    PARSE_STATE_SCHEME_START,
    PARSE_STATE_SCHEME,
    PARSE_STATE_SCHEME_END,
    PARSE_STATE_HOST_START,
    PARSE_STATE_HOST,
    PARSE_STATE_HOST_END,
    PARSE_STATE_PORT_START,
    PARSE_STATE_PORT,
    PARSE_STATE_PORT_END,
    PARSE_STATE_PATH_START,
    PARSE_STATE_PATH,
    PARSE_STATE_PATH_END,
    PARSE_STATE_SEARCHPART_START,
    PARSE_STATE_SEARCHPART,
    PARSE_STATE_SEARCHPART_END,
    PARSE_STATE_FRAGMENT_START,
    PARSE_STATE_FRAGMENT,
    PARSE_STATE_FRAGMENT_END
} PARSE_STATE;

static char *supportedSchemes[] = {"http", "https", "file", NULL};

CSEM_Url *CSEM_URL_Create() {
    CSEM_Url *url = CSEM_Calloc(1, sizeof(CSEM_Url));
    return url;
}
CSEM_Error CSEM_URL_Copy(const CSEM_Url *src, CSEM_Url **dist) {
    CSEM_Url *tmp = NULL;
    if(!src) {
        return CSEM_ERROR_PARAMETER;
    }
    if(!(tmp = CSEM_URL_Create())) {
        return CSEM_ERROR_MEMORY;
    }
    /* copy each member */
    tmp -> scheme = CSEM_Utils_Strcpy(src -> scheme);
    tmp -> hostname = CSEM_Utils_Strcpy(src -> hostname);
    tmp -> port = CSEM_Utils_Strcpy(src -> port);
    tmp -> path = CSEM_Utils_Strcpy(src -> path);
    tmp -> query = CSEM_Utils_Strcpy(src -> query);
    tmp -> fragment = CSEM_Utils_Strcpy(src -> fragment);

    *dist = tmp;
    return CSEM_ERROR_NONE;
}
static CSEM_Bool csem_url_isSupportedScheme(const char *urlstring) {
    int i = 0;
    char *supportedScheme = NULL;
    while((supportedScheme = supportedSchemes[i++])) {
        if(!strncmp(urlstring, supportedScheme, strlen(supportedScheme))) {
            return CSEM_TRUE;
        }
    }
    return CSEM_FALSE;
}
static CSEM_Bool csem_url_IsLowalpha(const unsigned char c) {
    return c >= 0x61U && c <= 0x7AU;/* [a-z] */
}
static CSEM_Bool csem_url_IsUpalpha(const unsigned char c) {
    return c >= 0x41U && c <= 0x5AU;/* [A-Z] */
}
static CSEM_Bool csem_url_IsAlpha(const unsigned char c) {
    return csem_url_IsLowalpha(c) || csem_url_IsUpalpha(c);
}
static CSEM_Bool csem_url_IsDigit(const unsigned char c) {
    return c >= 0x30U && c <= 0x39U;/* [0-9] */
}
static CSEM_Bool csem_url_IsHex(const unsigned char c) {
    /* hex            = digit | "A" | "B" | "C" | "D" | "E" | "F" |
                        "a" | "b" | "c" | "d" | "e" | "f" */
    return csem_url_IsDigit(c)
            || (c >= 0x41U && c <= 0x46U)  /* [A-F] */
            || (c >= 0x61U && c <= 0x66U); /* [a-f] */
}
static CSEM_Bool csem_url_IsUnreserved(const unsigned char c) {
    /* unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~" */
    return csem_url_IsAlpha(c) || csem_url_IsDigit(c) || c == '-' || c == '.' || c == '_' || c == '~';
}
static CSEM_Bool csem_url_IsPctEncoded(const unsigned char c) {
    /* pct-encoded   = "%" HEXDIG HEXDIG */
    return c == '%' || csem_url_IsHex(c);
}
static CSEM_Bool csem_url_IsSubDelims(const unsigned char c) {
    /* sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "=" */
    return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' ||
           c == '*' || c == '+' || c == ',' || c == ';' || c == '=';
}
static CSEM_Bool csem_url_IsPchar(const unsigned char c) {
    /* pchar         = unreserved / pct-encoded / sub-delims / ":" / "@" */
    return csem_url_IsUnreserved(c) || csem_url_IsPctEncoded(c) || csem_url_IsSubDelims(c) ||
            c == ':' || c == '@';
}
static CSEM_Bool csem_url_IsScheme(const unsigned char c) {
    /* scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
    return csem_url_IsAlpha(c) || csem_url_IsDigit(c) || c == '+' || c == '-' || c == '.';
}
static CSEM_Bool csem_url_IsHost(const unsigned char c) {
    /* host          = IP-literal / IPv4address / reg-name */
    return CSEM_TRUE;
}
static CSEM_Bool csem_url_IsPort(const unsigned char c) {
    /* port          = *DIGIT */
    return csem_url_IsDigit(c);
}
static CSEM_Bool csem_url_IsPath(const unsigned char c) {
    /* path          = path-abempty    ; begins with "/" or is empty
     *               / path-absolute   ; begins with "/" but not "//"
     *               / path-noscheme   ; begins with a non-colon segment
     *               / path-rootless   ; begins with a segment
     *               / path-empty      ; zero characters
     * path-abempty  = *( "/" segment )
     * path-absolute = "/" [ segment-nz *( "/" segment ) ]
     * path-noscheme = segment-nz-nc *( "/" segment )
     * path-rootless = segment-nz *( "/" segment )
     * path-empty    = 0<pchar>
     * segment       = *pchar */
    return csem_url_IsPchar(c) || c == '/';
}
static CSEM_Bool csem_url_IsQuery(const unsigned char c) {
    /* query         = *( pchar / "/" / "?" ) */
    return csem_url_IsPchar(c) || c == '/' || c == '?';
}
static CSEM_Bool csem_url_IsFragment(const unsigned char c) {
    /* fragment      = *( pchar / "/" / "?" ) */
    return csem_url_IsPchar(c) || c == '/' || c == '?';
}
CSEM_Error CSEM_URL_Parse(const char *urlstring, CSEM_Url **url) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Url *result;
    size_t i, len;
    int state = PARSE_STATE_SCHEME_START;
    size_t slashCount = 0;/* count '/' of "scheme://" */

    /* used to prevent buffer overflow */
    size_t index_scheme = 0;
    size_t index_host = 0;
    size_t index_port = 0;
    size_t index_path = 0;
    size_t index_query = 0;
    size_t index_fragment = 0;

    if(!urlstring || !url) {
        return CSEM_ERROR_PARAMETER;
    }
    *url = NULL;

    if(!(result = CSEM_URL_Create())) {
        return CSEM_ERROR_MEMORY;
    }

    /* scheme://<host>:<port>/<path>?<searchpart>
       httpurl        = "http://" host [ ":" port ] [ "/" hpath [ "?" search ]] */
    for(i = 0, len = strlen(urlstring); i < len; i++) {
        char c = urlstring[i];

        if(state == PARSE_STATE_SCHEME_START) {
            if(!csem_url_isSupportedScheme(urlstring)) {
                state = PARSE_STATE_PATH;
                if((result->path = CSEM_Calloc(MAXLEN_PATH + 1, sizeof(char))) == NULL) {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_MEMORY;
                }
                result->path[index_path++] = c;
                continue;
            }
            /* validate scheme */
            if(!csem_url_IsScheme(c)) {
                error = CSEM_ERROR_URL;
            }

            state = PARSE_STATE_SCHEME;
            if((result->scheme = CSEM_Calloc(MAXLEN_SCHEME + 1, sizeof(char))) == NULL) {
                CSEM_URL_Dispose(result);
                return CSEM_ERROR_MEMORY;
            }
            result->scheme[index_scheme++] = c;

        } else if(state == PARSE_STATE_SCHEME) {
            if(c == ':') {
                state = PARSE_STATE_SCHEME_END;
            } else {
                /* validate scheme */
                if(!csem_url_IsScheme(c)) {
                    error = CSEM_ERROR_URL;
                }
                if(index_scheme < MAXLEN_SCHEME) {
                    result->scheme[index_scheme++] = c;
                } else {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_TOO_LARGE_FIELD;
                }
            }
        } else if(state == PARSE_STATE_SCHEME_END) {
            if(!strncmp(result -> scheme, "file", 4)) {
                if(c == '/' && slashCount < 2) {
                    slashCount++;
                } else {
                    state = PARSE_STATE_PATH;
                    if((result->path = CSEM_Calloc(MAXLEN_PATH + 1, sizeof(char))) == NULL) {
                        CSEM_URL_Dispose(result);
                        return CSEM_ERROR_MEMORY;
                    }
                    result->path[index_path++] = c;
                }
            } else {
                if(c == '/') {
                    slashCount++;
                } else {
                    /* validate total '/' number of "://" */
                    if(slashCount != 2) {
                        error = CSEM_ERROR_URL;
                    }
                    /* validate host */
                    if(!csem_url_IsHost(c)) {
                        error = CSEM_ERROR_URL;
                    }
                    state = PARSE_STATE_HOST;
                    if((result->hostname = CSEM_Calloc(MAXLEN_HOST + 1, sizeof(char))) == NULL) {
                        CSEM_URL_Dispose(result);
                        return CSEM_ERROR_MEMORY;
                    }
                    result->hostname[index_host++] = c;
                }
            }
        } else if(state == PARSE_STATE_HOST) {
            if(c == ':') {
                state = PARSE_STATE_PORT_START;
            } else if(c == '/') {
                state = PARSE_STATE_PATH;
                if((result->path = CSEM_Calloc(MAXLEN_PATH + 1, sizeof(char))) == NULL) {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_MEMORY;
                }
                result->path[index_path++] = c;
            } else {
                /* validate host */
                if(!csem_url_IsHost(c)) {
                    error = CSEM_ERROR_URL;
                }
                if(index_host < MAXLEN_HOST) {
                    result->hostname[index_host++] = c;
                } else {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_TOO_LARGE_FIELD;
                }
            }
        } else if(state == PARSE_STATE_PORT_START) {
            /* validate port */
            if(!csem_url_IsPort(c)) {
                error = CSEM_ERROR_URL;
            }
            state = PARSE_STATE_PORT;
            /* port [0:65535] */
            if((result->port = CSEM_Calloc(MAXLEN_PORT + 1, sizeof(char))) == NULL) {
                CSEM_URL_Dispose(result);
                return CSEM_ERROR_MEMORY;
            }
            result->port[index_port++] = c;

        } else if(state == PARSE_STATE_PORT) {
            if(c == '/') {
                state = PARSE_STATE_PATH;
                if((result->path = CSEM_Calloc(MAXLEN_PATH + 1, sizeof(char))) == NULL) {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_MEMORY;
                }
                result->path[index_path++] = c;

            } else {
                /* validate port */
                if(!csem_url_IsPort(c)) {
                    error = CSEM_ERROR_URL;
                }
                if(index_port < MAXLEN_PORT) {
                    result->port[index_port++] = c;
                } else {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_TOO_LARGE_FIELD;
                }
            }
        } else if(state == PARSE_STATE_PATH) {
            if(c == '?') {
                state = PARSE_STATE_SEARCHPART_START;
            } else if(c == '#') {
                state = PARSE_STATE_FRAGMENT_START;
            } else {
                /* validate path */
                if(!csem_url_IsPath(c)) {
                    error = CSEM_ERROR_URL;
                }
                if(index_path < MAXLEN_PATH) {
                    result->path[index_path++] = c;
                } else {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_TOO_LARGE_FIELD;
                }
            }
        } else if(state == PARSE_STATE_SEARCHPART_START) {
            if(c == '#') {
                state = PARSE_STATE_FRAGMENT_START;
                error = CSEM_ERROR_URL;
            } else {
                /* validate query */
                if(!csem_url_IsQuery(c)) {
                    error = CSEM_ERROR_URL;
                }
                state = PARSE_STATE_SEARCHPART;
                if((result->query = CSEM_Calloc(MAXLEN_QUERY + 1, sizeof(char))) == NULL) {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_MEMORY;
                }
                result->query[index_query++] = c;
            }
        } else if(state == PARSE_STATE_SEARCHPART) {
            if(c == '#') {
                state = PARSE_STATE_FRAGMENT_START;
            } else {
                if(!csem_url_IsQuery(c)) {
                    error = CSEM_ERROR_URL;
                }
                if(index_query < MAXLEN_QUERY) {
                    result->query[index_query++] = c;
                } else {
                    CSEM_URL_Dispose(result);
                    return CSEM_ERROR_TOO_LARGE_FIELD;
                }
            }
        } else if(state == PARSE_STATE_FRAGMENT_START) {
            /* validate fragment */
            if(!csem_url_IsFragment(c)) {
                error = CSEM_ERROR_URL;
            }
            state = PARSE_STATE_FRAGMENT;

            if((result->fragment = CSEM_Calloc(MAXLEN_FRAGMENT + 1, sizeof(char))) == NULL) {
                CSEM_URL_Dispose(result);
                return CSEM_ERROR_MEMORY;
            }
            result->fragment[index_fragment++] = c;
        } else if(state == PARSE_STATE_FRAGMENT) {
            /* validate fragment */
            if(!csem_url_IsFragment(c)) {
                error = CSEM_ERROR_URL;
            }
            if(index_fragment < MAXLEN_FRAGMENT) {
                result->fragment[index_fragment++] = c;
            } else {
                CSEM_URL_Dispose(result);
                return CSEM_ERROR_TOO_LARGE_FIELD;
            }
        }
    }
    if(state == PARSE_STATE_SCHEME_END ||
       state == PARSE_STATE_PORT_START ||
       state == PARSE_STATE_SEARCHPART_START ||
       state == PARSE_STATE_FRAGMENT_START) {
        error = CSEM_ERROR_URL;
    }

    /* result */
    *url = result;

    return error;
}
/**
 *  Section 5.2.3 Merge Paths
 */
CSEM_Error csem_url_mergePath(const char *base, const char *relative, char **merged) {
    char *tmpMerged = NULL;
    char *last_slash = NULL;
    if(!base || base[0] == 0) {
        if(!(tmpMerged = CSEM_Calloc(strlen(relative) + 2, sizeof(char)))) {
            return CSEM_ERROR_MEMORY;
        }
        strcpy(tmpMerged, "/");
        strcat(tmpMerged, relative);
    } else {
        if(!relative) {
            if(!(tmpMerged = CSEM_Calloc(strlen(base) + 1, sizeof(char)))) {
                return CSEM_ERROR_MEMORY;
            }
            strcpy(tmpMerged, base);
        } else {
            last_slash = strrchr(base, '/');
            if(last_slash) {
                if(!(tmpMerged = CSEM_Calloc(last_slash - base + 1 + strlen(relative) + 1, sizeof(char)))) {
                    return CSEM_ERROR_MEMORY;
                }
                strncpy(tmpMerged, base, last_slash - base + 1);
                strcat(tmpMerged, relative);
            } else {
                if(!(tmpMerged = CSEM_Calloc(strlen(relative) + 1, sizeof(char)))) {
                    return CSEM_ERROR_MEMORY;
                }
                strcpy(tmpMerged, relative);
            }
        }
    }
    *merged = tmpMerged;
    return CSEM_ERROR_NONE;
}
/** part of Step 2-C */
static void csem_url_removeLastSegment(char *path) {
    char *slash = NULL;
    slash = strrchr(path, '/');
    if(slash) {
        *slash = 0;
    }
}
/**
 * Section 5.2.4 Remove Dot Segments
 */
CSEM_Error csem_url_removeDotSegments(char *inPath, char **outPath) {
    char *tmp;
    char *slash;

    if(!inPath) {
        *outPath = NULL;
        return CSEM_ERROR_NONE;
    }

    /* step 1 */
    if(!(tmp = CSEM_Calloc(strlen(inPath) + 1, sizeof(char)))) {
        return CSEM_ERROR_MEMORY;
    }
    /* step 2 */
    while(strlen(inPath)) {
        /* step 2-A */
        if(!strncmp(inPath, "../", 3)) {
            memmove(inPath, inPath + 3, strlen(inPath + 3) + 1);
            continue;
        } else if(!strncmp(inPath, "./", 2)) {
            memmove(inPath, inPath + 2, strlen(inPath + 2) + 1);
            continue;
        }

        /* step 2-B */
        if(!strncmp(inPath, "/./", 3)) {
            memmove(inPath + 1, inPath + 3, strlen(inPath + 3) + 1);
            continue;
        } else if(!strncmp(inPath, "/.", 3)) {
            memmove(inPath + 1, inPath + 2, strlen(inPath + 2) + 1);
            continue;
        }

        /* step 2-C */
        if(!strncmp(inPath, "/../", 4)) {
            memmove(inPath + 1, inPath + 4, strlen(inPath + 4) + 1);
            csem_url_removeLastSegment(tmp);
            continue;
        } else if(!strcmp(inPath, "/..")) {/* TODO : may be wrong */
            memmove(inPath + 1, inPath + 3, strlen(inPath + 3) + 1);
            csem_url_removeLastSegment(tmp);
            continue;
        }

        /* Step 2-D */
        if(!strcmp(inPath, ".") || !strcmp(inPath, "..")){
            inPath[0] = 0;
            continue;
        }

        /* Step 2-E */
        slash = strchr(inPath + 1, '/');
        if(slash) {
            strncat(tmp, inPath, slash - inPath);
            memmove(inPath, slash, strlen(slash) + 1);
        } else {/* final segment */
            strcat(tmp, inPath);
            break;
        }
    }
    /* Step 3 */
    *outPath = tmp;
    return CSEM_ERROR_NONE;
}
CSEM_Error CSEM_URL_Merge(const CSEM_Url *base, const CSEM_Url *relative, CSEM_Url **target) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Url *tmpTarget = NULL;
    char *tmpPath = NULL;
    if(!base  || !target) {
        return CSEM_ERROR_PARAMETER;
    }
    if(!relative) {
        error = CSEM_URL_Copy(base, target);
        return error;
    }

    tmpTarget = CSEM_URL_Create();

    if(relative -> scheme) {
        tmpTarget -> scheme = CSEM_Utils_Strcpy(relative -> scheme);
        tmpTarget -> hostname = CSEM_Utils_Strcpy(relative -> hostname);
        tmpTarget -> port = CSEM_Utils_Strcpy(relative -> port);
        if((error = csem_url_removeDotSegments(relative -> path, &(tmpTarget -> path)))) {
            goto ERROR;
        }
        tmpTarget -> query = CSEM_Utils_Strcpy(relative -> query);
    } else {
        if(relative -> hostname) {
            tmpTarget -> hostname = CSEM_Utils_Strcpy(relative -> hostname);
            tmpTarget -> port = CSEM_Utils_Strcpy(relative -> port);
            if((error = csem_url_removeDotSegments(relative -> path, &(tmpTarget -> path)))) {
                goto ERROR;
            }
            tmpTarget -> query = CSEM_Utils_Strcpy(relative -> query);
        } else {
            if(!(relative -> path) || relative -> path[0] == 0) {
                tmpTarget -> path = CSEM_Utils_Strcpy(base -> path);
                if(relative -> query) {
                    tmpTarget -> query = CSEM_Utils_Strcpy(relative -> query);
                } else {
                    tmpTarget -> query = CSEM_Utils_Strcpy(base -> query);
                }
            } else {
                if(relative -> path && relative -> path[0] == '/') {
                    if((error = csem_url_removeDotSegments(relative -> path, &(tmpTarget -> path)))) {
                        goto ERROR;
                    }
                } else {
                    if((error = csem_url_mergePath(base -> path, relative -> path, &tmpPath))) {
                        goto ERROR;
                    }
                    if((error = csem_url_removeDotSegments(tmpPath, &(tmpTarget -> path)))) {
                        goto ERROR;
                    }
                    CSEM_Free(tmpPath);
                }
                tmpTarget -> query = CSEM_Utils_Strcpy(relative -> query);
            }
            tmpTarget -> hostname = CSEM_Utils_Strcpy(base -> hostname);
            tmpTarget -> port = CSEM_Utils_Strcpy(base -> port);
        }
        tmpTarget -> scheme = CSEM_Utils_Strcpy(base -> scheme);
    }
    tmpTarget -> fragment = CSEM_Utils_Strcpy(relative -> fragment);

    *target = tmpTarget;
    return error;

ERROR:
    CSEM_Free(tmpPath);
    CSEM_URL_Dispose(tmpTarget);
    return error;
}
CSEM_Error CSEM_URL_Serialize(const CSEM_Url *url, char **urlstring) {
    CSEM_Error error = CSEM_ERROR_NONE;
    char *tmp = NULL;
    size_t tmpLen = 0;
    if(!url || !urlstring) {
        return CSEM_ERROR_PARAMETER;
    }
    *urlstring = NULL;
    tmpLen = MAXLEN_SCHEME + MAXLEN_HOST + MAXLEN_PORT +
                MAXLEN_PATH + MAXLEN_QUERY + MAXLEN_FRAGMENT;
    if(!(tmp = CSEM_Calloc(tmpLen, sizeof(char)))) {
        return CSEM_ERROR_MEMORY;
    }

    if(url -> scheme) {
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 2, url -> scheme, "://")) {
            goto ERROR;
        }
    }
    if(url -> hostname) {
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 1, url -> hostname)) {
            goto ERROR;
        }
    }
    if(url -> port) {
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 2, ":", url -> port)) {
            goto ERROR;
        }
    }
    if(url -> path) {
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 1, url -> path)) {
            goto ERROR;
        }
    }
    if(url -> query) {
        if(!(url -> path)) {
            if(!CSEM_Utils_Strcat(tmp, tmpLen, 1, "/")) {
                goto ERROR;
            }
        }
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 2, "?", url -> query)) {
            goto ERROR;
        }
    }
    if(url -> fragment) {
        if(!(url -> path) && !(url -> query)) {
            if(!CSEM_Utils_Strcat(tmp, tmpLen, 1, "/")) {
                goto ERROR;
            }
        }
        if(!CSEM_Utils_Strcat(tmp, tmpLen, 2, "#", url -> fragment)) {
            goto ERROR;
        }
    }

    /* result */
    *urlstring = tmp;

    return error;

ERROR:
    CSEM_Free(tmp);
    return CSEM_ERROR_TOO_LARGE_FIELD;
}
void CSEM_URL_Dispose(CSEM_Url *url) {
    if(url) {
        CSEM_Free(url -> scheme);
        CSEM_Free(url -> hostname);
        CSEM_Free(url -> port);
        CSEM_Free(url -> path);
        CSEM_Free(url -> query);
        CSEM_Free(url -> fragment);
        CSEM_Free(url);
    }
}
