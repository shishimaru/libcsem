/* $Id$ */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

#define INDENT(n) {int i=0;for(i=0;i<n;i++){printf("  ");}}

/**
 * Demo of HTML5 microdata API.
 */
void showProperties(CSEM_List *properties, int depth);
void showItem(CSEM_Item *item, int depth) {
    if(item) {
        int j = 0;
        CSEM_Url *itemidURL = CSEM_Item_GetId(item);
        CSEM_List *types = CSEM_Item_GetTypes(item);
        CSEM_List *refs = CSEM_Item_GetRefs(item);
        CSEM_List *properties = CSEM_Item_GetProperties(item);

        INDENT(depth);puts("ITEM {");
        if(itemidURL) {
            char *itemid = NULL;
            CSEM_URL_Serialize(itemidURL, &itemid);
            INDENT(depth);printf("@itemid=%s\n", itemid);
            free(itemid);
        }
        for(j = 0; types && j < CSEM_List_Size(types); j++) {
            INDENT(depth);printf("@itemtype=%s\n", (char *)CSEM_List_Get(types, j));
        }
        for(j = 0; refs && j < CSEM_List_Size(refs); j++) {
            INDENT(depth);printf("@itemref=%s\n", (char *)CSEM_List_Get(refs, j));
        }
        showProperties(properties, depth);
        INDENT(depth);puts("}");
    }
}
void showId(CSEM_Id *id, int depth) {
    if(id) {
        char *id_value = CSEM_Id_GetId(id);
        CSEM_List *properties = CSEM_Id_GetProperties(id);

        INDENT(depth);puts("ID {");
        if(id_value) {
            INDENT(depth);printf("@id=%s\n", id_value);
        }
        showProperties(properties, depth);
        INDENT(depth);puts("}");
    }
}
CSEM_Error showProperty(CSEM_Property *property, int depth) {
    CSEM_Error error = CSEM_ERROR_NONE;
    if(property) {
        int i = 0;
        CSEM_List *names = CSEM_Property_GetNames(property);
        CSEM_List *values = NULL, *types = NULL;
        if((error = CSEM_Property_GetValues(property, &values, &types))) {
            goto FINISH;
        }
        for(i = 0; names && i < CSEM_List_Size(names); i++) {
            INDENT(depth);printf("@itemprop=%s\n", (char *)CSEM_List_Get(names, i));
        }
        for(i = 0; values && i < CSEM_List_Size(values); i++) {
            int *type = CSEM_List_Get(types, i);

            if(*type == CSEM_VALUE_TYPE_STR || *type == CSEM_VALUE_TYPE_URL) {
                INDENT(depth);printf("\"%s\"\n", (char *)CSEM_List_Get(values, i));
            } else if(*type == CSEM_VALUE_TYPE_ITEM) {
                CSEM_Item *item = CSEM_List_Get(values, i);
                showItem(item, depth + 1);
            } else if(*type == CSEM_VALUE_TYPE_PROPERTY) {
                CSEM_Property *property = CSEM_List_Get(values, i);
                showProperty(property, depth + 1);
            }
        }
    }
FINISH:
    return error;
}
void showProperties(CSEM_List *properties, int depth) {
    int i = 0;
    for(i = 0; properties && i < CSEM_List_Size(properties); i++) {
        CSEM_Property *property = CSEM_List_Get(properties, i);
        showProperty(property, depth);
    }
}
int main(int argc, char *argv[]) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = NULL;
    CSEM_Url *baseURL = NULL;
    CSEM_Document *doc = NULL;
    char *filename = NULL;
    char *type = NULL;
    char *itemprop = NULL;
    int fd = -1;

    /* setup input parameters */
    if(argc < 3) {
        error = CSEM_ERROR_PARAMETER;
        printf("./sample [filename] [itemtype] [itemprop]\n");
        goto FINISH;
    }
    filename = argv[1];
    type = argv[2];
    itemprop = argv[3];

    /* init */
    if((error = CSEM_Builder_Create(&builder))) {
        goto FINISH;
    }
    /* set base url */
    if((error = CSEM_URL_Parse("http://localhost/test/", &baseURL))) {
        goto FINISH;
    }
    CSEM_Builder_SetBaseURL(builder, baseURL);
    /* start build process */
    if(!(fd = open(filename, O_RDONLY))) {
        fprintf(stderr, "failed to open %s\n", filename);
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd))) {
        fprintf(stderr, "failed parsing %s\n", filename);
        goto FINISH;
    }
    if((error = CSEM_Builder_GetDocument(builder, &doc))) {
        goto FINISH;
    }
    {/* sample of document.getItems([types]) */
        int i = 0;
        CSEM_List *items = NULL;
        CSEM_List *types = NULL;
        if(!(types = CSEM_List_Create(8))) {
            goto FINISH_GET_ITEMS;
        }
        if((error = CSEM_List_Add(types, type))) {
            goto FINISH_GET_ITEMS;
        }
        /* get items which has specified itemtype */
        if((error = CSEM_Document_GetItems(doc, types, &items))) {
            goto FINISH_GET_ITEMS;
        }
        /* get values which has specified itemprop */
        for(i = 0; i < CSEM_List_Size(items); i++) {
            int j = 0;
            CSEM_List *properties = NULL;
            CSEM_List *values = NULL, *valueTypes = NULL;
            CSEM_Item *item = CSEM_List_Get(items, i);

            if((error = CSEM_Item_GetNamedPropertes(item, itemprop, &properties))) {
                goto FINISH_NAMED_PROPERTIES;
            }
            if((error = CSEM_Properties_GetValues(properties, &values, &valueTypes))) {
                goto FINISH_NAMED_PROPERTIES;
            }
            /* show values */
            for(j = 0; j < CSEM_List_Size(values); j++) {
                int *valueType = CSEM_List_Get(valueTypes, j);
                if(*valueType == CSEM_VALUE_TYPE_STR || *valueType == CSEM_VALUE_TYPE_URL) {
                    printf("\"%s\"\n", (char *)CSEM_List_Get(values, j));
                } else if(*valueType == CSEM_VALUE_TYPE_ITEM) {
                    showItem(CSEM_List_Get(values, j), 0);
                } else if(*valueType == CSEM_VALUE_TYPE_PROPERTY) {
                    showProperty(CSEM_List_Get(values, j), 0);
                }
            }
FINISH_NAMED_PROPERTIES:
            CSEM_List_Dispose(properties, CSEM_FALSE);
            CSEM_List_Dispose(values, CSEM_FALSE);
            CSEM_List_Dispose(valueTypes, CSEM_FALSE);
        }
FINISH_GET_ITEMS:
        CSEM_List_Dispose(types, CSEM_FALSE);
        CSEM_List_Dispose(items, CSEM_FALSE);
    }
FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
    CSEM_URL_Dispose(baseURL);
    puts("END");
    return error ? 1 : 0;
}
