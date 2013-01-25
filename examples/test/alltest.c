/*$Id$*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Util.h>

#include "test_utils.h"
#include "test_url.h"
#include "test_list.h"
#include "test_micro_stream.h"
#include "test_micro_tree.h"

static CU_TestInfo testinfo_url[] = {
        //parse absolute url
        { "test url : scheme", test_url_scheme},
        { "test url : scheme host", test_url_scheme_host},
        { "test url : scheme host port", test_url_scheme_host_port},
        { "test url : scheme host port path", test_url_scheme_host_port_path},
        { "test url : scheme host port path query", test_url_http_scheme_host_port_path_query},

        { "test url : scheme host path", test_url_scheme_host_path},
        { "test url : scheme host path query", test_url_http_scheme_host_path_query},
        { "test url : scheme host path fragment", test_url_http_scheme_host_path_fragment},
        { "test url : scheme host path query fragment", test_url_http_scheme_host_path_query_fragment},
        { "test url : file scheme path", test_url_file_path},
        { "test url : file scheme path query", test_url_file_path},
        { "test url : file scheme path fragment", test_url_file_path_fragment},
        { "test url : file scheme path query fragment", test_url_file_path_query_fragment},

        //parse relative url
        { "test url : path", test_url_path},
        { "test url : path query", test_url_http_path_query},
        { "test url : path fragment", test_url_http_path_fragment},
        { "test url : path query fragment", test_url_http_path_query_fragment},

        //parser error
        { "test url error : scheme", test_url_error_scheme},
        { "test url error : scheme host", test_url_error_scheme_host},
        { "test url error : scheme host port", test_url_error_scheme_host_port},
        { "test url error : scheme host port path", test_url_error_scheme_host_port_path},
        { "test url error : scheme host port path query", test_url_http_error_scheme_host_port_path_query},
        { "test url error : scheme host port path query segment", test_url_http_error_scheme_host_port_path_query_segment},

        { "test url error : long scheme", test_url_error_long_scheme},
        { "test url error : long host", test_url_error_long_host},
        { "test url error : long port", test_url_error_long_port},
        { "test url error : long path", test_url_error_long_path},
        { "test url error : long query", test_url_http_error_long_query},
        { "test url error : long fragment", test_url_http_error_long_fragment},

        //resolve relative url
        {"test url merge : merge path", test_url_merge_mergePath},
        {"test url merge : remove dot segments", test_url_merge_removeDotSegments},
        {"test url merge : merge url", test_url_merge_mergeUrl},
        {"test url merge : merge abnormal url", test_url_merge_mergeAbnormalUrl},

        //writer
        { "test url writer", test_url_writer},

        //writer error
        { "test url writer error : long scheme", test_url_writer_error_long_scheme},
        { "test url writer error : long host", test_url_writer_error_long_host},
        { "test url writer error : long port", test_url_writer_error_long_port},
        { "test url writer error : long path", test_url_writer_error_long_path},
        { "test url writer error : long query", test_url_http_writer_error_long_query},
        { "test url writer error : long fragment", test_url_http_writer_error_long_fragment},

        CU_TEST_INFO_NULL,
};
static CU_TestInfo testinfo_utils[] = {
        { "test utils : strcpy", test_utils_strcpy},
        { "test utils : strncpy", test_utils_strncpy},
        { "test utils : strcat", test_utils_strcat},
        { "test utils : split", test_utils_strtoks},
        { "test utils : nsplit", test_utils_strntoks},
        { "test utils : unquote", test_utils_unquote},
        { "test utils : tolower", test_utils_tolower},
        { "test utils : tostring", test_utils_tostring},

        CU_TEST_INFO_NULL,
};
static CU_TestInfo testinfo_list[] = {
        { "test list : basic feature", test_list},
        { "test list : clear feature", test_list_clear},
        { "test list : dispose", test_list_dispose},

        CU_TEST_INFO_NULL,
};
static CU_TestInfo testinfo_microdata[] = {
        /*{ "test stream parser : basic", test_microdata_stream_basic},
        { "test stream parser : Section5.4 values", test_microdata_stream_values},
        { "test tree parser   : recursive itemprop", test_microdata_tree_recursive_itemprop},
        { "test tree parser   : basic", test_microdata_tree_basic_no_resolve},
        { "test tree parser   : values", test_microdata_tree_values},
        { "test tree parser   : no microdata", test_microdata_tree_no_microdata},
        { "test tree parser   : simplify", test_microdata_tree_basic_resolve},
        { "test tree parser   : simplify recursive items", test_microdata_tree_resolve_recursive_items},
        { "test tree parser   : schema.org/event/ex1", test_microdata_tree_schema_org_event_ex1},
        { "test tree parser   : schema.org/event/ex2", test_microdata_tree_schema_org_event_ex2},//*/
        { "test tree parser   : schema.org/person", test_microdata_tree_schema_org_person},
        /*{ "test microdata API : document.getItems(NULL)", test_microdata_tree_getItems_no_types},
        { "test microdata API : document.getItems([types])", test_microdata_tree_getItems_with_types},
        { "test microdata API : item.getNamedPropertries(name)", test_microdata_tree_getNamedProperties},
        { "test microdata API : item.getNamedPropertries(NULL)", test_microdata_tree_getNamedProperties_no_name},
        { "test microdata API : properties.getValues()", test_microdata_tree_properties_getValues},//*/

        CU_TEST_INFO_NULL,
};
static CU_SuiteInfo suites[] = {
        /*{ "utils",  NULL, NULL, testinfo_utils},
        { "list", NULL, NULL, testinfo_list},
        { "url",  NULL, NULL, testinfo_url},*/
        { "microdata", NULL, NULL, testinfo_microdata},

        CU_SUITE_INFO_NULL,
};
#define MODE_ALL    0
#define MODE_SAMPLE 1
#define MODE_STUDY  2
#define EXECUTE MODE_ALL

int main(int argc, char *argv[]) {

#if(EXECUTE == MODE_ALL)
    CU_initialize_registry();
    CU_register_suites(suites);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

#elif(EXECUTE == MODE_SAMPLE)
    main_small_test_tree();

#elif(EXECUTE == MODE_STUDY)


#endif

    puts("--- test end ---");
    return EXIT_SUCCESS;
}
