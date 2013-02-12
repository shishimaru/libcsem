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
 * @mainpage HTML5 microdata/RDFa-Lite Library
 * <p>HTML5 microdata/RDFa-Lite Library enables native applications to
 * extract them from HTML5 with high performance and small heap size.</p>
 * <h3>Function</h3>
 * <ul>
 *   <li>Parser for <a href="http://www.w3.org/TR/2012/WD-microdata-20121025/">HTML5 microdata</a>,
 *       <a href="http://www.w3.org/TR/2012/REC-rdfa-lite-20120607/">RDFa Lite 1.1</a>
 *   </li>
 *   <ul>
 *     <li>Light weight streaming parser</li>
 *     <ul>
 *       <li>Parse chunk html little by little, and when microdata/RDFa-Lite is detected,
 *       the corresponding events are passed via callback handlers</li>
 *       <li>Because currently the parsers work on SAX parser and don't create
 *       large DOM tree internally, applications can reduce the memory foot print
 *     </ul>
 *     <li>Data model builder</li>
 *     <ul>
 *       <li>Create a data model based on the parsed result of underlying streaming parser</li>
 *       <li>All semantics of microdata/RDFa-Lite can be accessed via
 *       <a href="http://www.w3.org/html/wg/drafts/microdata/master/#microdata-dom-api">HTML5 microdata API</a></li>
 *     </ul>
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
