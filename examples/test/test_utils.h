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

extern void util_timer_start();
extern double util_timer_end();
extern int util_apache_stop();
extern int util_apache_start();

void test_utils_strcpy();
void test_utils_strncpy();
void test_utils_strcat();
void test_utils_strtoks();
void test_utils_strntoks();
void test_utils_unquote();
void test_utils_tolower();
void test_utils_tostring();
//void test_utils_sha1();
//void test_utils_md5();
//void test_utils_md5string();

void test_utils_convertUINT64();
void test_utils_convertUINT32();
void test_utils_convertUINT16();

CSEM_NS_C_END
#endif /* TEST_UTILS_H */
