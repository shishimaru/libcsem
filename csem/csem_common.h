/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_COMMON_H
#define CSEM_COMMON_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN
/**
 * @mainpage HTML5 microdata Library
 * <p>HTML5 microdata Library enables native applications to
 * extract microdata from HTML5 with high performance and small heap size.</p>
 * <h3>Function</h3>
 * <ul>
 *   <li><a href="http://www.w3.org/html/wg/drafts/microdata/master/">HTML5 microdata</a> parser</li>
 *   <ul>
 *     <li>Light weight streaming parser</li>
 *     <ul>
 *       <li>Parse chunk html little by little, and when microdata is detected,
 *       the corresponding events are passed via registered callback handlers</li>
 *       <li>Because currently the parser works on SAX parser and don't create
 *       large DOM tree internally, applications can reduce the memory foot print
 *     </ul>
 *     <li>Data model builder</li>
 *     <ul>
 *       <li>Create a data model for microdata based on the parsed result of underlying streaming parser</li>
 *       <li>All microdata can be accessed via HTML5 microdata API</li>
 *     </ul>
 *     <li><a href="http://www.w3.org/html/wg/drafts/microdata/master/#microdata-dom-api">microdata API</a></li>
 *   </ul>
 * </ul>
 * <h3>Platform</h3>
 * <ul>
 *   <li>Ubuntu12.04</li>
 *   <li>Dependency</li>
 *   <ul>
 *     <li>ANSI C API</li>
 *     <li><a href="http://www.xmlsoft.org/">libxml</a></li>
 *   </ul>
 * </ul>
 * @author Hitoshi Uchida <uchida@w3.org>
 */
#include <stdint.h>
/** Default size of internal buffer. */
#define CSEM_DEFAULT_BUF_SIZE 1024
/** Default allowed max size of attribute value. */
#define CSEM_ALLOWED_MAX_ATTR_VALUE_SIZE 1024
/** CSEM boolean type. */
typedef enum CSEM_Bool {
    CSEM_FALSE = 0,
    CSEM_TRUE = 1
} CSEM_Bool;

/** Error Codes. */
typedef enum CSEM_Error {
    /** No Error **/
    CSEM_ERROR_NONE = 0,
    /** Error of parameters */
    CSEM_ERROR_PARAMETER,
    /** Error of memory allocation */
    CSEM_ERROR_MEMORY,
    /** Error of parsing */
    CSEM_ERROR_PARSE,
    /** Error of url parsing */
    CSEM_ERROR_URL,
    /** Error of too large field */
    CSEM_ERROR_TOO_LARGE_FIELD
} CSEM_Error;

CSEM_NS_C_END

#endif /* CSEM_COMMON_H */
