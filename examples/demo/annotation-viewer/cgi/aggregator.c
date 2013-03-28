#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"
#include "utils.h"

static int itemNum = 0;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *userdata) {
    CSEM_Builder *builder = userdata;
    CSEM_Builder_ParseChunk(builder, ptr, size * nmemb, !size);
    return size * nmemb;
}
static CSEM_Error get(char *url, CSEM_Document **doc) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Document *res = NULL;

    CSEM_Builder *builder = NULL;
    CSEM_Url *baseURL = NULL;
    CURL *curl = NULL;
    CURLcode curlres = CURLE_OK;

    /* init libcsem */
    if((error = CSEM_Builder_Create(&builder))) {
        goto FINISH;
    }
    /* set base url */
    if((error = CSEM_URL_Parse(url, &baseURL))) {
        goto FINISH;
    }
    CSEM_Builder_SetBaseURL(builder, baseURL);
    /* init libcurl */
    if(!(curl = curl_easy_init())) {
        error = CSEM_ERROR_MEMORY;
        goto FINISH;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, builder);

    /* start build process */
    if((curlres = curl_easy_perform(curl))) {
        error = 1;
        goto FINISH;
    }
    CSEM_Builder_ParseChunk(builder, NULL, 0, 1);
    CSEM_Builder_GetDocument(builder, &res);
    *doc = res;

FINISH:
    curl_easy_cleanup(curl);
    CSEM_Builder_Dispose(builder);
    CSEM_URL_Dispose(baseURL);
    return error;
}
void showProperties(CSEM_List *properties, cJSON *parent);
void showItem(CSEM_Item *item, cJSON *json_parent) {
    if(item) {
        CSEM_Url *itemidURL = CSEM_Item_GetId(item);
        int j = 0;
        CSEM_List *types = CSEM_Item_GetTypes(item);
        CSEM_List *refs = CSEM_Item_GetRefs(item);
        CSEM_List *properties = CSEM_Item_GetProperties(item);

        cJSON *json_obj = cJSON_CreateObject();

        if(itemidURL) {
            char *itemid = NULL;
            CSEM_URL_Serialize(itemidURL, &itemid);
            char itemName[256];
            strcpy(itemName, "__ITEM__:");
            strcat(itemName, itemid);
            cJSON_AddItemToObject(json_parent, itemName, json_obj);
            free(itemid);
        } else {
            char blankName[32];
            sprintf(blankName, "__ITEM__:_%d", itemNum++);
            cJSON_AddItemToObject(json_parent, blankName, json_obj);
        }
        if(types) {
            cJSON *json_array = cJSON_CreateArray();
            for(j = 0; types && j < CSEM_List_Size(types); j++) {
                cJSON *json_str = cJSON_CreateString(CSEM_List_Get(types, j));
                cJSON_AddItemToArray(json_array, json_str);
            }
            cJSON_AddItemToObject(json_obj, "__ATTR__:type", json_array);
        }
        if(refs) {
            cJSON *json_array = cJSON_CreateArray();
            for(j = 0; refs && j < CSEM_List_Size(refs); j++) {
                cJSON *json_str = cJSON_CreateString(CSEM_List_Get(refs, j));
                cJSON_AddItemToArray(json_array, json_str);
            }
            cJSON_AddItemToObject(json_obj, "__ATTR__:refs", json_array);
        }
        showProperties(properties, json_obj);
    }
}
CSEM_Error showProperty(CSEM_Property *property, cJSON *json_properties) {
    CSEM_Error error = CSEM_ERROR_NONE;

    if(property) {
        int i = 0;
        CSEM_List *names = CSEM_Property_GetNames(property);
        CSEM_List *values = NULL, *types = NULL;
        cJSON *json_prop_values = cJSON_CreateArray();
        char names_str[256];
        memset(names_str, 0, sizeof(names_str));

        if((error = CSEM_Property_GetValues(property, &values, &types))) {
            goto FINISH;
        }
        if(names) {
            for(i = 0; names && i < CSEM_List_Size(names); i++) {
                if(i) {
                    strcat(names_str, ",");
                }
                strcat(names_str, CSEM_List_Get(names, i));
            }
        }
        for(i = 0; values && i < CSEM_List_Size(values); i++) {
            cJSON *json_prop_value = NULL;
            int *type = CSEM_List_Get(types, i);

            if(*type == CSEM_VALUE_TYPE_STR) {
                json_prop_value = cJSON_CreateString(CSEM_List_Get(values, i));
            } else if(*type == CSEM_VALUE_TYPE_URL) {
                char value_str[256];
                strcpy(value_str, "__URL__:");
                strcat(value_str, CSEM_List_Get(values, i));
                json_prop_value = cJSON_CreateString(value_str);
            } else if(*type == CSEM_VALUE_TYPE_ITEM) {
                CSEM_Item *item = CSEM_List_Get(values, i);
                json_prop_value = cJSON_CreateObject();
                showItem(item, json_prop_value);
            } else if(*type == CSEM_VALUE_TYPE_PROPERTY) {
                CSEM_Property *property = CSEM_List_Get(values, i);
                json_prop_value = cJSON_CreateObject();
                showProperty(property, json_prop_value);
            }
            cJSON_AddItemToArray(json_prop_values, json_prop_value);
        }
        cJSON_AddItemToObject(json_properties, names_str, json_prop_values);
    }
FINISH:
    return error;
}
void showProperties(CSEM_List *properties, cJSON *json_obj) {
    cJSON *json_properties = cJSON_CreateObject();
    cJSON_AddItemToObject(json_obj, "__ATTR__:properties", json_properties);
    if(properties) {
        int i = 0;
        for(i = 0; properties && i < CSEM_List_Size(properties); i++) {
            CSEM_Property *property = CSEM_List_Get(properties, i);
            showProperty(property, json_properties);
        }
    }
}
static void buildJSON(CSEM_Document *doc, cJSON *root) {
    if(doc) {
        int i = 0;
        CSEM_List *nodes = CSEM_Document_GetChildren(doc);
        for(i = 0; i < CSEM_List_Size(nodes); i++) {
            CSEM_Node *node = CSEM_List_Get(nodes, i);

            if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_ITEM) {
                CSEM_Item *item = CSEM_Node_GetObject(node);
                showItem(item, root);
            } else if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_ID) {
                /*CSEM_Id *id = CSEM_Node_GetObject(node);
                showId(id, 0);*/
            } else if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_PROPERTY) {
                CSEM_Property *property = CSEM_Node_GetObject(node);
                showProperty(property, root);
            }
        }
    }
}
static void show(CSEM_Document *doc) {
    cJSON *root = cJSON_CreateObject();
    buildJSON(doc, root);
    {
        char *json_str = cJSON_Print(root);
        printf("%s\n", json_str);
        free(json_str);
    }
    cJSON_Delete(root);
}
void serialize(char *url) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Document *doc = NULL;

    if((error = get(url, &doc))) {
        printf("{\"error\" : \"failed to retrieve\"}\n");
        goto ERROR;
    }
    if(doc) {
        show(doc);
    } else {
        printf("{\"error\" : \"failed to parse\"}\n");
    }
ERROR:
    CSEM_Document_Dispose(doc);
}

int main(int argc, char *argv[]) {
    int i = 0;
    CSEM_List *queries = NULL;
    printf("Content-Type: application/json\n\n");
    {
        char *query_str = getenv("QUERY_STRING");
        if(!query_str || !strcmp(query_str, "")) {
            query_str = argv[1];
        }
        Utils_Strtoks(query_str, "&", &queries);
    }
    for(i = 0; i < CSEM_List_Size(queries); i++) {
        char *query = CSEM_List_Get(queries, i);
        if(!strncmp(query, "url=", 4)) {
            char *url = query + 4;
            serialize(url);
        }
    }
    CSEM_List_Dispose(queries, CSEM_TRUE);
    return 0;
}
