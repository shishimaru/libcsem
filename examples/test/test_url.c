#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>

#include "csem/csem_url.h"

void test_url_scheme() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("http://", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_PTR_NULL(url->path);
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("file://", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_PTR_NULL(url->path);
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{//check
        //scheme         = 1*[ lowalpha | digit | "+" | "-" | "." ]
        char c = 0;
        for(c = 0; c < 0x7F; c++) {
            if((c >= 0x30 && c <= 0x39) //[0-9]
                    || (c >= 0x41 && c <= 0x5A) //[A-Z]
                    || (c >= 0x61 && c <= 0x7A) //[a-z]
                    || (c == '+' || c == '-' || c == '.')) { // "+" | "-" | "."
                char buf[2] = {c,0};
                CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                CU_ASSERT_PTR_NULL(url->scheme);
                CU_ASSERT_PTR_NULL(url->hostname);
                CU_ASSERT_PTR_NULL(url->port);
                CU_ASSERT_NSTRING_EQUAL(url->path, buf, 1);
                CU_ASSERT_PTR_NULL(url->query);
                CU_ASSERT_PTR_NULL(url->fragment);

                //dispose
                CSEM_URL_Dispose(url);
            }
        }
    }
}
void test_url_scheme_host() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("http://localhost", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_PTR_NULL(url->path);
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
//    {
//        CSEM_URL_Parse("file://localhost", &url);
//
//        //check
//        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
//        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
//        CU_ASSERT_PTR_NULL(url->port);
//        CU_ASSERT_PTR_NULL(url->path);
//        CU_ASSERT_PTR_NULL(url->query);
//        CU_ASSERT_PTR_NULL(url->fragment);
//
//        //dispose
//        CSEM_URL_Dispose(url);
//    }
}
void test_url_scheme_host_port() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("http://localhost:80", &url);

        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_STRING_EQUAL(url->port, "80");
        CU_ASSERT_PTR_NULL(url->path);
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
//    {
//        CSEM_URL_Parse("file://localhost:80", &url);
//
//        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
//        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
//        CU_ASSERT_STRING_EQUAL(url->port, "80");
//        CU_ASSERT_PTR_NULL(url->path);
//        CU_ASSERT_PTR_NULL(url->query);
//        CU_ASSERT_PTR_NULL(url->fragment);
//
//        //dispose
//        CSEM_URL_Dispose(url);
//    }
    {
        //port         = digit
        char c = 0;
        for(c = 0; c < 0x7F; c++) {
            if((c >= 0x30 && c <= 0x39)) { //[0-9]
                char buf[19];
                memset(buf, 0, sizeof(buf));
                strcat(buf, "http://localhost:");
                buf[17] = c;

                CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                CU_ASSERT_STRING_EQUAL(url->scheme, "http");
                CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
                CU_ASSERT_STRING_EQUAL(url->port, buf+17);
                CU_ASSERT_PTR_NULL(url->path);
                CU_ASSERT_PTR_NULL(url->query);
                CU_ASSERT_PTR_NULL(url->fragment);

                //dispose
                CSEM_URL_Dispose(url);
            }
        }
    }
}
void test_url_scheme_host_port_path() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("http://localhost:80/index.html", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_STRING_EQUAL(url->port, "80");
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
//    {
//        CSEM_URL_Parse("file://localhost:80/index.html", &url);
//
//        //check
//        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
//        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
//        CU_ASSERT_STRING_EQUAL(url->port, "80");
//        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
//        CU_ASSERT_PTR_NULL(url->query);
//        CU_ASSERT_PTR_NULL(url->fragment);
//
//        //dispose
//        CSEM_URL_Dispose(url);
//    }
    {
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
         *
         * segment       = *pchar
         * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
         * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
         * pct-encoded   = "%" HEXDIG HEXDIG
         * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
         * */
        char c = 0;
        for(c = 0; c < 0x7F; c++) {
            if((c >= 0x41 && c <= 0x5A) //[a-z]
                    || (c >= 0x61 && c <= 0x7A) //[A-Z]
                    || (c >= 0x30 && c <= 0x39) //[0-9]
                    || (c=='-' || c=='.' || c=='_' || c=='~')//"-" / "." / "_" / "~"
                    || (c=='%')
                    || (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='=') //"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    || (c==':' || c=='@')//":" / "@"
                    || (c=='/')) {

                char buf[20];
                memset(buf, 0, sizeof(buf));
                strcat(buf, "http://a:0/");
                buf[11] = c;

                CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                CU_ASSERT_STRING_EQUAL(url->scheme, "http");
                CU_ASSERT_STRING_EQUAL(url->hostname, "a");
                CU_ASSERT_STRING_EQUAL(url->port, "0");
                CU_ASSERT_STRING_EQUAL(url->path, buf+10);
                CU_ASSERT_PTR_NULL(url->query);
                CU_ASSERT_PTR_NULL(url->fragment);

                //dispose
                CSEM_URL_Dispose(url);
            }
        }
   }
}
void test_url_http_scheme_host_port_path_query() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("http://localhost:80/index.html?a=1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_STRING_EQUAL(url->port, "80");
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        /* query         = *( pchar / "/" / "?" )
         * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
         * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
         * pct-encoded   = "%" HEXDIG HEXDIG
         * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
         */
        char c = 0;
        for(c = 0; c < 0x7F; c++) {
            if((c >= 0x41 && c <= 0x5A) //[a-z]
                    || (c >= 0x61 && c <= 0x7A) //[A-Z]
                    || (c >= 0x30 && c <= 0x39) //[0-9]
                    || (c=='-' || c=='.' || c=='_' || c=='~')//"-" / "." / "_" / "~"
                    || (c=='%')
                    || (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='=') //"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    || (c==':' || c=='@')//":" / "@"
                    || (c=='/' || c=='?')){//"/" / "?"
                char buf[20];
                memset(buf, 0, sizeof(buf));
                strcat(buf, "http://a:0/a?");
                buf[13] = c;

                CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                CU_ASSERT_STRING_EQUAL(url->scheme, "http");
                CU_ASSERT_STRING_EQUAL(url->hostname, "a");
                CU_ASSERT_STRING_EQUAL(url->port, "0");
                CU_ASSERT_STRING_EQUAL(url->path, "/a");
                CU_ASSERT_STRING_EQUAL(url->query, buf+13);
                CU_ASSERT_PTR_NULL(url->fragment);

                //dispose
                CSEM_URL_Dispose(url);
            }
        }
    }
}
void test_url_scheme_host_path() {
    CSEM_Url *url;
    CSEM_URL_Parse("http://localhost/index.html", &url);

    //check
    CU_ASSERT_STRING_EQUAL(url->scheme, "http");
    CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
    CU_ASSERT_PTR_NULL(url->port);
    CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
    CU_ASSERT_PTR_NULL(url->query);

    //dispose
    CSEM_URL_Dispose(url);
}
void test_url_http_scheme_host_path_query() {
    CSEM_Url *url; {
        CSEM_URL_Parse("http://localhost/index.html?a=1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("http://localhost/?a=1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{//bug#20
        CU_ASSERT_NOT_EQUAL(CSEM_URL_Parse("https://sample.org/aa/zz/asp/WXZZA820.asp?URI=http://sample2.org/", &url), CSEM_ERROR_URL);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "https");
        CU_ASSERT_STRING_EQUAL(url->hostname, "sample.org");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/aa/zz/asp/WXZZA820.asp");
        CU_ASSERT_STRING_EQUAL(url->query, "URI=http://sample2.org/");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{//bug#28
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http://www.bing.com/fd/ls/l?IG=015606b6ae2f46a68bdfedd9b22bf678&CID=E197DB91E1E04C3D8A4E4B6CCE26A4EC&PM=Y&Type=Event.CPT&DATA={%22pp%22:{%22S%22:%22L%22,%22PC%22:15,%22FC%22:-1,%22BC%22:15,%22BS%22:29,%22H%22:42,%22FE%22:-1,%22LE%22:-1,%22C1%22:-1,%22C2%22:-1,%22BP%22:157,%22KP%22:-1,%22CT%22:2490,%22IL%22:0}}&P=SERP&DA=SG1", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "www.bing.com");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/fd/ls/l");
        CU_ASSERT_STRING_EQUAL(url->query, "IG=015606b6ae2f46a68bdfedd9b22bf678&CID=E197DB91E1E04C3D8A4E4B6CCE26A4EC&PM=Y&Type=Event.CPT&DATA={%22pp%22:{%22S%22:%22L%22,%22PC%22:15,%22FC%22:-1,%22BC%22:15,%22BS%22:29,%22H%22:42,%22FE%22:-1,%22LE%22:-1,%22C1%22:-1,%22C2%22:-1,%22BP%22:157,%22KP%22:-1,%22CT%22:2490,%22IL%22:0}}&P=SERP&DA=SG1");
        CU_ASSERT_PTR_NULL(url->fragment);
        CSEM_URL_Dispose(url);

        CU_ASSERT_EQUAL(CSEM_URL_Parse("http://web-jp.ad-v.jp/adam/detect?req=fr&cat=jiji-press.travel&&&BROWSER=SAFARI&LOC=http://www.jiji.com/jc/v4\\qid=power_generation010001\\arel=y\\ag=phl&WIDTH=1024&HEIGHT=800&WIDTH_RANGE=WR_D&DATE=01110901&HOUR=13&ORD=19562612217850983", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "web-jp.ad-v.jp");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/adam/detect");
        CU_ASSERT_STRING_EQUAL(url->query, "req=fr&cat=jiji-press.travel&&&BROWSER=SAFARI&LOC=http://www.jiji.com/jc/v4\\qid=power_generation010001\\arel=y\\ag=phl&WIDTH=1024&HEIGHT=800&WIDTH_RANGE=WR_D&DATE=01110901&HOUR=13&ORD=19562612217850983");
        CU_ASSERT_PTR_NULL(url->fragment);
        CSEM_URL_Dispose(url);
    }
}
void test_url_http_scheme_host_path_fragment() {
    CSEM_Url *url; {
        CSEM_URL_Parse("http://localhost/index.html#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("http://localhost/#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_http_scheme_host_path_query_fragment() {
    CSEM_Url *url; {
        CSEM_URL_Parse("http://localhost/index.html?a=1#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("http://localhost/index.html?a#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "http");
        CU_ASSERT_STRING_EQUAL(url->hostname, "localhost");
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a");
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_file_path() {
    CSEM_Url *url; {
        CSEM_URL_Parse("file:///tmp/test/hoge", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/tmp/test/hoge");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("file://./test/hoge", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "./test/hoge");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_file_path_fragment() {
    CSEM_Url *url; {
        CSEM_URL_Parse("file:///tmp/test/hoge#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/tmp/test/hoge");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_file_path_query_fragment() {
    CSEM_Url *url; {
        CSEM_URL_Parse("file:///tmp/test/hoge?a=1&b=2#section1", &url);

        //check
        CU_ASSERT_STRING_EQUAL(url->scheme, "file");
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/tmp/test/hoge");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1&b=2");
        CU_ASSERT_STRING_EQUAL(url->fragment, "section1");

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_path() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("/index.html", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("index.html", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("./index.html", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "./index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("../../index.html", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "../../index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_http_path_query() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("/index.html?a=1", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("index.html?a=1", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("./index.html?a=1", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "./index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("../../index.html?a=1", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "../../index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_PTR_NULL(url->fragment);

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_http_path_fragment() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("/index.html#a", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "a");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("index.html#a", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "a");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("./index.html#a", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "./index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "a");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("../../index.html#a", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "../../index.html");
        CU_ASSERT_PTR_NULL(url->query);
        CU_ASSERT_STRING_EQUAL(url->fragment, "a");

        //dispose
        CSEM_URL_Dispose(url);
    }
}
void test_url_http_path_query_fragment() {
    CSEM_Url *url;
    {
        CSEM_URL_Parse("/index.html?a=1#b", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "/index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_STRING_EQUAL(url->fragment, "b");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("index.html?a=1#b", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_STRING_EQUAL(url->fragment, "b");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("./index.html?a=1#b", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "./index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_STRING_EQUAL(url->fragment, "b");

        //dispose
        CSEM_URL_Dispose(url);
    }{
        CSEM_URL_Parse("../../index.html?a=1#b", &url);

        //check
        CU_ASSERT_PTR_NULL(url->scheme);
        CU_ASSERT_PTR_NULL(url->hostname);
        CU_ASSERT_PTR_NULL(url->port);
        CU_ASSERT_STRING_EQUAL(url->path, "../../index.html");
        CU_ASSERT_STRING_EQUAL(url->query, "a=1");
        CU_ASSERT_STRING_EQUAL(url->fragment, "b");

        //dispose
        CSEM_URL_Dispose(url);
    }
}

void test_url_error_scheme() {
    CSEM_Url *url;

    {//check NULL
        CU_ASSERT_EQUAL(CSEM_URL_Parse(NULL, &url), CSEM_ERROR_PARAMETER);
    }{//check NULL
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http://", NULL), CSEM_ERROR_PARAMETER);
    }{//unknown scheme
        CU_ASSERT_EQUAL(CSEM_URL_Parse("xxx://", NULL), CSEM_ERROR_PARAMETER);
    }
}
void test_url_error_scheme_host() {
    CSEM_Url *url;
    {
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http:localhost", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
        CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
        CU_ASSERT_EQUAL(url -> port, NULL);
        CU_ASSERT_EQUAL(url -> path, NULL);
        CU_ASSERT_EQUAL(url -> query, NULL);
        CU_ASSERT_EQUAL(url -> fragment, NULL);
        CSEM_URL_Dispose(url);
    }{
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http:/localhost", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
        CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
        CU_ASSERT_EQUAL(url -> port, NULL);
        CU_ASSERT_EQUAL(url -> path, NULL);
        CU_ASSERT_EQUAL(url -> query, NULL);
        CU_ASSERT_EQUAL(url -> fragment, NULL);
        CSEM_URL_Dispose(url);
    }{
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http:///localhost", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
        CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
        CU_ASSERT_EQUAL(url -> port, NULL);
        CU_ASSERT_EQUAL(url -> path, NULL);
        CU_ASSERT_EQUAL(url -> query, NULL);
        CU_ASSERT_EQUAL(url -> fragment, NULL);
        CSEM_URL_Dispose(url);
    }
}
void test_url_error_scheme_host_port() {
    CSEM_Url *url;
    {
        //port         = digit
        char c = 0;
        for(c = 0x00; c < 0x7F; c++) {
            if((c >= 0x30 && c <= 0x39)) { //[0-9]
                //skip
            } else {
                {
                    char buf[19];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://localhost:");
                    buf[17] = c;

                    CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
                    if(c == 0) {
                        CU_ASSERT_EQUAL(url -> port, NULL);
                    } else {
                        CU_ASSERT_STRING_EQUAL(url -> port, buf + 17);
                    }
                    CU_ASSERT_EQUAL(url -> path, NULL);
                    CU_ASSERT_EQUAL(url -> query, NULL);
                    CU_ASSERT_EQUAL(url -> fragment, NULL);
                    CSEM_URL_Dispose(url);
                }
                {
                    char buf[20];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://localhost:1");
                    buf[18] = c;

                    if(c == 0 || c == '/') {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                    } else {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    }
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
                    if(c == '/') {
                        buf[18] = 0;
                        CU_ASSERT_STRING_EQUAL(url -> port, buf + 17);
                        CU_ASSERT_STRING_EQUAL(url -> path, "/");
                    } else {
                        CU_ASSERT_STRING_EQUAL(url -> port, buf + 17);
                        CU_ASSERT_EQUAL(url -> path, NULL);
                    }
                    CU_ASSERT_EQUAL(url -> query, NULL);
                    CU_ASSERT_EQUAL(url -> fragment, NULL);
                    CSEM_URL_Dispose(url);
                }
            }
        }
    }
}
void test_url_error_scheme_host_port_path() {
    CSEM_Url *url;
    {
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
         *
         * segment       = *pchar
         * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
         * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
         * pct-encoded   = "%" HEXDIG HEXDIG
         * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
         */
        char c = 0;
        for(c = 0x01; c < 0x7F; c++) {
            if((c >= 0x41 && c <= 0x5A) //[a-z]
                    || (c >= 0x61 && c <= 0x7A) //[A-Z]
                    || (c >= 0x30 && c <= 0x39) //[0-9]
                    || (c=='-' || c=='.' || c=='_' || c=='~')//"-" / "." / "_" / "~"
                    || (c=='%')
                    || (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='=') //"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    || (c==':' || c=='@')//":" / "@"
                    || (c=='/')) {
                //skip
            } else {
                char buf[20];
                memset(buf, 0, sizeof(buf));
                strcat(buf, "http://a:0/");
                buf[11] = c;

                CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                CU_ASSERT_STRING_EQUAL(url -> hostname, "a");
                CU_ASSERT_STRING_EQUAL(url -> port, "0");
                if(c == '#' || c == '?') {
                    buf[11] = 0;
                }
                CU_ASSERT_STRING_EQUAL(url -> path, buf + 10);
                CU_ASSERT_EQUAL(url -> query, NULL);
                CU_ASSERT_EQUAL(url -> fragment, NULL);
                CSEM_URL_Dispose(url);
            }
        }
   }
}
void test_url_http_error_scheme_host_port_path_query() {
    CSEM_Url *url;
    {
        /* query         = *( pchar / "/" / "?" )
         * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
         * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
         * pct-encoded   = "%" HEXDIG HEXDIG
         * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
         */
        char c = 0;
        for(c = 0x00; c < 0x7F; c++) {
            if((c >= 0x41 && c <= 0x5A) //[a-z]
                    || (c >= 0x61 && c <= 0x7A) //[A-Z]
                    || (c >= 0x30 && c <= 0x39) //[0-9]
                    || (c=='-' || c=='.' || c=='_' || c=='~')//"-" / "." / "_" / "~"
                    || (c=='%')
                    || (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='=') //"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    || (c==':' || c=='@')//":" / "@"
                    || (c=='/' || c=='?')){//"/" / "?"
                //skip
            } else {
                {
                    char buf[20];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://a:0/a?");
                    buf[13] = c;

                    CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "a");
                    CU_ASSERT_STRING_EQUAL(url -> port, "0");
                    CU_ASSERT_STRING_EQUAL(url -> path, "/a");
                    if(c == 0 || c == '#') {
                        CU_ASSERT_EQUAL(url -> query, NULL);
                    }else {
                        CU_ASSERT_STRING_EQUAL(url -> query, buf + 13);
                    }
                    CU_ASSERT_EQUAL(url -> fragment, NULL);
                    CSEM_URL_Dispose(url);
                }
                {
                    char buf[20];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://a:0/a?a");
                    buf[14] = c;

                    if(c == 0) {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                    } else {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    }
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "a");
                    CU_ASSERT_STRING_EQUAL(url -> port, "0");
                    CU_ASSERT_STRING_EQUAL(url -> path, "/a");
                    if(c == '#') {
                        buf[14] = 0;
                    }
                    CU_ASSERT_STRING_EQUAL(url -> query, buf + 13);
                    CU_ASSERT_EQUAL(url -> fragment, NULL);
                    CSEM_URL_Dispose(url);
                }
            }
        }
    }
}
void test_url_http_error_scheme_host_port_path_query_segment() {
    CSEM_Url *url;
    {
        CU_ASSERT_EQUAL(CSEM_URL_Parse("http://localhost/index.html?#section1", &url), CSEM_ERROR_URL);
        CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
        CU_ASSERT_STRING_EQUAL(url -> hostname, "localhost");
        CU_ASSERT_EQUAL(url -> port, NULL);
        CU_ASSERT_STRING_EQUAL(url -> path, "/index.html");
        CU_ASSERT_EQUAL(url -> query, NULL);
        CU_ASSERT_STRING_EQUAL(url -> fragment, "section1");
        CSEM_URL_Dispose(url);
    }{
        /* fragment      = *( pchar / "/" / "?" )
         * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
         * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
         * pct-encoded   = "%" HEXDIG HEXDIG
         * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
         */
        char c = 0;
        for(c = 0x00; c < 0x7F; c++) {
            if((c >= 0x41 && c <= 0x5A) //[a-z]
                    || (c >= 0x61 && c <= 0x7A) //[A-Z]
                    || (c >= 0x30 && c <= 0x39) //[0-9]
                    || (c=='-' || c=='.' || c=='_' || c=='~')//"-" / "." / "_" / "~"
                    || (c=='%')
                    || (c=='!' || c=='$' || c=='&' || c=='\'' || c=='(' || c==')' || c=='*' || c=='+' || c==',' || c==';' || c=='=') //"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    || (c==':' || c=='@')//":" / "@"
                    || (c=='/' || c=='?')){//"/" / "?"
                //skip
            } else {
                {
                    char buf[20];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://a:0/a?a#");
                    buf[15] = c;

                    CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "a");
                    CU_ASSERT_STRING_EQUAL(url -> port, "0");
                    CU_ASSERT_STRING_EQUAL(url -> path, "/a");
                    CU_ASSERT_STRING_EQUAL(url -> query, "a");
                    if(c == 0) {
                        CU_ASSERT_EQUAL(url -> fragment, NULL);
                    }else {
                        CU_ASSERT_STRING_EQUAL(url -> fragment, buf + 15);
                    }
                    CSEM_URL_Dispose(url);
                }
                {
                    char buf[20];
                    memset(buf, 0, sizeof(buf));
                    strcat(buf, "http://a:0/a?a#a");
                    buf[16] = c;

                    if(c == 0) {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_NONE);
                    } else {
                        CU_ASSERT_EQUAL(CSEM_URL_Parse(buf, &url), CSEM_ERROR_URL);
                    }
                    CU_ASSERT_STRING_EQUAL(url -> scheme, "http");
                    CU_ASSERT_STRING_EQUAL(url -> hostname, "a");
                    CU_ASSERT_STRING_EQUAL(url -> port, "0");
                    CU_ASSERT_STRING_EQUAL(url -> path, "/a");
                    CU_ASSERT_STRING_EQUAL(url -> query, "a");
                    CU_ASSERT_STRING_EQUAL(url -> fragment, buf + 15);
                    CSEM_URL_Dispose(url);
                }
            }
        }
    }
}
void test_url_error_long_scheme() {
    {// length of scheme is 16
        char urlstring[1024];
        memset(urlstring, 0, sizeof(urlstring));

        int i = 0;
        for(i = 0; i < 16; i++) {
            strcat(urlstring, "a");
        }
        strcat(urlstring, "://localhost/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);
    }{// length of scheme is 17
        char urlstring[1024];
        memset(urlstring, 0, sizeof(urlstring));

        int i = 0;
        for(i = 0; i < 17; i++) {
            strcat(urlstring, "a");
        }
        strcat(urlstring, "://localhost/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);
    }
}
void test_url_error_long_host() {
    {// length of host is 1024
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://");
        int i = 0;
        for(i = 0; i < 1024; i++) {
            strcat(urlstring, "a");
        }
        strcat(urlstring, "/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);

    }{//length of host is 1025
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://");
        int i = 0;
        for(i = 0; i < 1025; i++) {
            strcat(urlstring, "a");
        }
        strcat(urlstring, "/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_TOO_LARGE_FIELD);
    }
}
void test_url_error_long_port() {
    {// length of port is 8
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost:");
        int i = 0;
        for(i = 0; i < 8; i++) {
            strcat(urlstring, "1");
        }
        strcat(urlstring, "/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);
    }
    {//length of port is 9
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost:");
        int i = 0;
        for(i = 0; i < 9; i++) {
            strcat(urlstring, "1");
        }
        strcat(urlstring, "/");

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_TOO_LARGE_FIELD);
    }
}
void test_url_error_long_path() {
    {// length of path is 1024
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/");
        int i = 0;
        for(i = 0; i < 1023; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);

    }{//length of path is 1025
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/");
        int i = 0;
        for(i = 0; i < 1024; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_TOO_LARGE_FIELD);
    }
}
void test_url_http_error_long_query() {
    {// length of query is 1024
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/?");
        int i = 0;
        for(i = 0; i < 1024; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);

    }{//length of query is 1025
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/?");
        int i = 0;
        for(i = 0; i < 1025; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_TOO_LARGE_FIELD);
    }
}
void test_url_http_error_long_fragment() {
    {// length of fragment is 1024
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/#");
        int i = 0;
        for(i = 0; i < 1024; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_NONE);
        CSEM_URL_Dispose(url);

    }{//length of fragmet is 1025
        char urlstring[2048];
        memset(urlstring, 0, sizeof(urlstring));

        strcat(urlstring, "http://localhost/#");
        int i = 0;
        for(i = 0; i < 1025; i++) {
            strcat(urlstring, "a");
        }

        CSEM_Url *url;
        CU_ASSERT_EQUAL(CSEM_URL_Parse(urlstring, &url), CSEM_ERROR_TOO_LARGE_FIELD);
    }
}
void test_url_merge_mergePath() {
    char *base, *relative, *merged;
    {
        base = NULL;
        relative = "a";
        csem_url_mergePath(base, relative, &merged);
        CU_ASSERT_STRING_EQUAL(merged, "/a");
        free(merged);
    }{
        base = "";
        relative = "a";
        csem_url_mergePath(base, relative, &merged);
        CU_ASSERT_STRING_EQUAL(merged, "/a");
        free(merged);
    }{
        base = "a/b";
        relative = "c";
        csem_url_mergePath(base, relative, &merged);
        CU_ASSERT_STRING_EQUAL(merged, "a/c");
        free(merged);
    }{
        base = "a/b/c";
        relative = "d/e";
        csem_url_mergePath(base, relative, &merged);
        CU_ASSERT_STRING_EQUAL(merged, "a/b/d/e");
        free(merged);
    }{
        base = "a";
        relative = "b/c";
        csem_url_mergePath(base, relative, &merged);
        CU_ASSERT_STRING_EQUAL(merged, "b/c");
        free(merged);
    }
}
void test_url_merge_removeDotSegments() {
    char inPath[128], *outPath;
    {
        memset(inPath, 0, sizeof(inPath));
        strcpy(inPath, "/a/b/c/./../../g");
        csem_url_removeDotSegments(inPath, &outPath);
        CU_ASSERT_STRING_EQUAL(outPath, "/a/g");
        free(outPath);
    }{
        memset(inPath, 0, sizeof(inPath));
        strcpy(inPath, "mid/content=5/../6");
        csem_url_removeDotSegments(inPath, &outPath);
        CU_ASSERT_STRING_EQUAL(outPath, "mid/6");
        free(outPath);
    }{
        memset(inPath, 0, sizeof(inPath));
        strcpy(inPath, "/b/c/g");
        csem_url_removeDotSegments(inPath, &outPath);
        CU_ASSERT_STRING_EQUAL(outPath, "/b/c/g");
        free(outPath);
    }
}
void test_url_merge_mergeUrl() {
    CSEM_Url *base, *relative, *merged;
    char *baseString, *relativeString, *mergedString;
    {
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("./g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "./g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g/", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g/");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }/*{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("//g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }*//*{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("?y", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/d;p?y");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }*/{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g?y", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g?y");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g?y");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }/*{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("#s", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/d;p?q#s");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }*/{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g?y#s", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g?y#s");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g?y#s");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse(";x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/;x");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, ";x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g;x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g;x");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g;x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g;x?y#s", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/g;x?y#s");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "g;x?y#s");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/d;p?q");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Merge(base, NULL, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/d;p?q");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse(".", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, ".");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("./", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "./");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("..", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "..");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "../");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/g");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "../g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../..", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "../..");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../../", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "../../");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/g");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "../../g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        relative = CSEM_URL_Create(); {
            relative -> hostname = (char *)CSEM_Utils_Strcpy("p");
            relative -> path = (char *)CSEM_Utils_Strcpy("/g");
        }
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://p/g");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "p/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        relative = CSEM_URL_Create(); {
            relative -> query = (char *)CSEM_Utils_Strcpy("Q");
        }
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &mergedString);
        CU_ASSERT_STRING_EQUAL(mergedString, "http://a/b/c/d;p?Q");

        CSEM_URL_Serialize(base, &baseString);
        CU_ASSERT_STRING_EQUAL(baseString, "http://a/b/c/d;p?q");
        CSEM_URL_Serialize(relative, &relativeString);
        CU_ASSERT_STRING_EQUAL(relativeString, "/?Q");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(baseString);
        free(relativeString);
        free(mergedString);
    }
}
void test_url_merge_mergeAbnormalUrl() {
    CSEM_Url *base, *relative, *merged;
    char *urlstring;
    {
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../../../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("../../../../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("/./g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("/../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g.", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g.");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g..", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g..");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("..g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/..g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("./../g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("./g/.", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g/");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g/./h", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g/h");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g/../h", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/h");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g;x=1/./y", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g;x=1/y");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g;x=1/../y", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/y");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g?y/./x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g?y/./x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g?y/../x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g?y/../x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g#s/./x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g#s/./x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("g#s/../x", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://a/b/c/g#s/../x");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }{
        CSEM_URL_Parse("http://a/b/c/d;p?q", &base);
        CSEM_URL_Parse("http:g", &relative);
        CSEM_URL_Merge(base, relative, &merged);

        CSEM_URL_Serialize(merged, &urlstring);
        CU_ASSERT_STRING_EQUAL(urlstring, "http://g");

        CSEM_URL_Dispose(base);
        CSEM_URL_Dispose(relative);
        CSEM_URL_Dispose(merged);
        free(urlstring);
    }
}

void test_url_writer() {
    CSEM_Url url;
    char *result;
    {
        CU_ASSERT_EQUAL(CSEM_URL_Serialize(NULL, &result), CSEM_ERROR_PARAMETER);
        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, NULL), CSEM_ERROR_PARAMETER);
    }{//full compornent for http://
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/test?a=1#section1");
        free(result);
    }{//full component for file://
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "file";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test/1.html";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "file://localhost:8000/test/1.html");
        free(result);
    }{//no scheme
        memset(&url, 0, sizeof(CSEM_Url));
        //url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "localhost:8000/test?a=1#section1");
        free(result);
    }{//no host
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        //url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://:8000/test?a=1#section1");
        free(result);
    }{//no host&port
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        //url.hostname = "localhost";
        //url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http:///test?a=1#section1");
        free(result);
    }{//no port
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        //url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost/test?a=1#section1");
        free(result);
    }{//no path
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        //url.path = "/test";
        url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/?a=1#section1");
        free(result);
    }{//no path&query
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        //url.path = "/test";
        //url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/#section1");
        free(result);
    }{//no path&fragment
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        //url.path = "/test";
        url.query = "a=1";
        //url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/?a=1");
        free(result);
    }{//no path&query&fragment
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        //url.path = "/test";
        //url.query = "a=1";
        //url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000");
        free(result);
    }{//no query
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        //url.query = "a=1";
        url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/test#section1");
        free(result);
    }{//no fragment
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        url.query = "a=1";
        //url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/test?a=1");
        free(result);
    }{//no query&fragment
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = "http";
        url.hostname = "localhost";
        url.port = "8000";
        url.path = "/test";
        //url.query = "a=1";
        //url.fragment = "section1";

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_NONE);
        CU_ASSERT_STRING_EQUAL(result, "http://localhost:8000/test");
        free(result);
    }
}
void test_url_writer_error_long_scheme() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.scheme = calloc(1024*10, sizeof(char));
        for(i = 0; i < 1024*10 - 1; i++) {
            url.scheme[i] = 'a';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.scheme);
    }
}
void test_url_writer_error_long_host() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.hostname = calloc(1024*10, sizeof(char));
        for(i = 0; i < 1024*10 - 1; i++) {
            url.hostname[i] = 'a';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.hostname);
    }
}
void test_url_writer_error_long_port() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.port = calloc(1024*10, sizeof(char));
        for(i = 0; i < 1024*10 - 1; i++) {
            url.port[i] = '1';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.port);
    }
}
void test_url_writer_error_long_path() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.path = calloc(1024*10, sizeof(char));
        url.path[0] = '/';
        for(i = 1; i < 1024*10 - 1; i++) {
            url.path[i] = 'a';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.path);
    }
}
void test_url_http_writer_error_long_query() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.query = calloc(1024*10, sizeof(char));
        for(i = 0; i < 1024*10 - 1; i++) {
            url.query[i] = '1';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.query);
    }
}
void test_url_http_writer_error_long_fragment() {
    CSEM_Url url;
    char *result;
    int i;
    {
        memset(&url, 0, sizeof(CSEM_Url));
        url.fragment = calloc(1024*10, sizeof(char));
        for(i = 0; i < 1024*10 - 1; i++) {
            url.fragment[i] = 'a';
        }

        CU_ASSERT_EQUAL(CSEM_URL_Serialize(&url, &result), CSEM_ERROR_TOO_LARGE_FIELD);
        CU_ASSERT_EQUAL(result, NULL);
        free(url.fragment);
    }
}
