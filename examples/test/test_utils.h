/* $Id$ */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

void test_utils_strcpy();
void test_utils_strncpy();
void test_utils_strcat();
void test_utils_strtoks();
void test_utils_strntoks();
void test_utils_strhash();
void test_utils_unquote();
void test_utils_tolower();
void test_utils_tostring();

CSEM_NS_C_END
#endif /* TEST_UTILS_H */
