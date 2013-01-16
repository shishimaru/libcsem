#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "csem_utils.h"

void test_utils_strcpy() {
    char *out = NULL;

    {
        char *in = NULL;
        out = CSEM_Utils_Strcpy(in);
        CU_ASSERT_EQUAL(out, NULL);
    }{
        char *in = "";
        out = CSEM_Utils_Strcpy(in);
        CU_ASSERT_STRING_EQUAL(out, "");
        free(out);
    }{
        char *in = "a";
        out = CSEM_Utils_Strcpy(in);
        CU_ASSERT_STRING_EQUAL(out, "a");
        free(out);
    }
}
void test_utils_strncpy() {
    char *out = NULL;

    {
        char *in = NULL;
        out = CSEM_Utils_Strncpy(in, 0);
        CU_ASSERT_EQUAL(out, NULL);
    }{
        char *in = "";
        out = CSEM_Utils_Strncpy(in, strlen(in));
        CU_ASSERT_STRING_EQUAL(out, "");
        free(out);
    }{
        char *in = "a";
        out = CSEM_Utils_Strncpy(in, strlen(in));
        CU_ASSERT_STRING_EQUAL(out, "a");
        free(out);
    }
}
void test_utils_strcat() {
    char to[8];
    CSEM_Bool result;

    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 0, "a");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 1, "");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 1, "a");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "a");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 1, "1234567");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "1234567");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 1, "12345678");
        CU_ASSERT_EQUAL(result, CSEM_FALSE);
        CU_ASSERT_STRING_EQUAL(to, "");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 1, "a", "b");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "a");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 2, "a", "b");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "ab");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 2, "1234", "567");
        CU_ASSERT_EQUAL(result, CSEM_TRUE);
        CU_ASSERT_STRING_EQUAL(to, "1234567");
    }
    {
        memset(to, 0, sizeof(to));
        result = CSEM_Utils_Strcat(to, sizeof(to), 2, "1234", "5678");
        CU_ASSERT_EQUAL(result, CSEM_FALSE);
        CU_ASSERT_STRING_EQUAL(to, "1234");
    }
}
void test_utils_strtoks() {
    {
        const char *delim = " ";
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "aa");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 1);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }{
        const char *delim = " ";
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 0);
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }{
        const char *delim = " ";
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(NULL, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 0);
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    } {
        const char *delim = NULL;
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "aa");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 1);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }{
        const char *delim = " ";
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "aa bb cc");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 3);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 1), "bb");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 2), "cc");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }{
        const char *delim = NULL;
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "aa bb cc");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 1);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa bb cc");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }{
        const char *delim = "\t\n";
        CSEM_List *tokens = NULL;
        char src[1024]; {
            strcpy(src, "aa\tbb\ncc dd");
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strtoks(src, delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 3);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 1), "bb");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 2), "cc dd");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    }
}
void test_utils_strntoks() {
    {
        const char *delim = "\t\n";
        CSEM_List *tokens = NULL;
        char *src = "aa\tbb\ncc dd";
        char buf[1024]; {
            strcpy(buf, src);
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strntoks(buf, strlen(src), delim, &tokens), CSEM_ERROR_NONE);
        CU_ASSERT_EQUAL(CSEM_List_Size(tokens), 3);
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 0), "aa");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 1), "bb");
        CU_ASSERT_STRING_EQUAL(CSEM_List_Get(tokens, 2), "cc dd");
        CSEM_List_Dispose(tokens, CSEM_TRUE);
    } {
        const char *delim = "\t\n";
        CSEM_List *tokens = NULL;
        char *src = "aa\tbb\ncc dd";
        char buf[1024]; {
            strcpy(buf, src);
        }

        CU_ASSERT_EQUAL(CSEM_Utils_Strntoks(buf, CSEM_DEFAULT_BUF_SIZE, delim, &tokens), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(tokens, NULL);
    }
}
void test_utils_unquote() {
    char *out = NULL;
    char in[1024];
    memset(in, 0, sizeof(in));

    {
        char *src = "a";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "a");
    }
    {
        char *src = "aa";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "aa");
    }
    {
        char *src = "aaa";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "aaa");
    }
    {
        char *src = "\"";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "\"");
    }
    {
        char *src = "\" ";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "\" ");
    }
    {
        char *src = " \"";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, " \"");
    }
    {
        char *src = "\"\"";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "");
    }
    {
        char *src = "\"a\"";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "a");
    }
    {
        char *src = "\"aa\"";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "aa");
    }
    {
        char *src = "'";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "'");
    }
    {
        char *src = "' ";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "' ");
    }
    {
        char *src = " '";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, " '");
    }
    {
        char *src = "''";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "");
    }
    {
        char *src = "'a'";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "a");
    }
    {
        char *src = "'aa'";
        strcpy(in, src);
        CSEM_Utils_Unquote(in, strlen(src), &out);
        CU_ASSERT_STRING_EQUAL(out, "aa");
    }
}
void test_utils_tolower() {
    char in[1024];
    memset(in, 0, sizeof(in));

    {
        strcpy(in, "ABCabc1234");
        CSEM_Utils_ToLower(in);
        CU_ASSERT_STRING_EQUAL(in, "abcabc1234");
    }
}
void test_utils_tostring() {
    char *out;
    unsigned char in[4];
    int value;

    {
        value = 0;
        memcpy(in, &value, sizeof(value));
        CSEM_Utils_ToString(in, 4, &out);
        CU_ASSERT_STRING_EQUAL(out, "00000000");
        free(out);
    }
    {
        value = 1;
        memcpy(in, &value, sizeof(value));
        CSEM_Utils_ToString(in, 4, &out);
        CU_ASSERT_STRING_EQUAL(out, "01000000");
        free(out);
    }
    {
        value = 1;
        memcpy(in, &value, sizeof(value));
        CSEM_Utils_ToString(in, 3, &out);
        CU_ASSERT_STRING_EQUAL(out, "010000");
        free(out);
    }
    {
        value = 0xffffffff;
        memcpy(in, &value, sizeof(value));
        CSEM_Utils_ToString(in, 4, &out);
        CU_ASSERT_STRING_EQUAL(out, "ffffffff");
        free(out);
    }
}
