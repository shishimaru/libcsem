//parse absolute
void test_url_scheme();
void test_url_scheme_host();
void test_url_scheme_host_port();
void test_url_scheme_host_port_path();
void test_url_http_scheme_host_port_path_query();
void test_url_scheme_host_path();
void test_url_http_scheme_host_path_query();
void test_url_http_scheme_host_path_fragment();
void test_url_http_scheme_host_path_query_fragment();
void test_url_file_path();
void test_url_file_path_fragment();
void test_url_file_path_query_fragment();

//parse relative
void test_url_path();
void test_url_http_path_query();
void test_url_http_path_fragment();
void test_url_http_path_query_fragment();

//errorneous
void test_url_error_scheme();
void test_url_error_scheme_host();
void test_url_error_scheme_host_port();
void test_url_error_scheme_host_port_path();
void test_url_http_error_scheme_host_port_path_query();
void test_url_http_error_scheme_host_port_path_query_segment();

void test_url_error_long_scheme();
void test_url_error_long_host();
void test_url_error_long_port();
void test_url_error_long_path();
void test_url_http_error_long_query();
void test_url_http_error_long_fragment();

//merge
void test_url_merge_mergePath();
void test_url_merge_removeDotSegments();
void test_url_merge_mergeUrl();
void test_url_merge_mergeAbnormalUrl();

//equal
void test_url_equal_scheme();
void test_url_equal_scheme_host();
void test_url_equal_scheme_host_port();
void test_url_equal_scheme_host_port_path();
void test_url_equal_scheme_host_port_path_query();
void test_url_equal_scheme_host_port_path_fragment();
void test_url_equal_scheme_host_port_path_query_fragment();
void test_url_equal_null();

//writer
void test_url_writer();
void test_url_writer_error_long_scheme();
void test_url_writer_error_long_host();
void test_url_writer_error_long_port();
void test_url_writer_error_long_path();
void test_url_http_writer_error_long_query();
void test_url_http_writer_error_long_fragment();

