/*$id$*/
#include <stdio.h>
#include <fcntl.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

/**
 * Demo of HTML5 microdata API.
 */
int main(int argc, char *argv[]) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_Builder *builder = NULL;
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
    /* start build process */
    if(!(fd = open(filename, O_RDONLY))) {
        fprintf(stderr, "failed to open %s\n", filename);
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd, &doc, CSEM_TRUE))) {
        fprintf(stderr, "failed parsing %s\n", filename);
        goto FINISH;
    }
    {/* sample of document.getItems([types]) */
        int i = 0;
        CSEM_List *items = NULL;
        CSEM_List *types = CSEM_List_Create(8);
        CSEM_List_Add(types, type);

        /* get items which has specified itemtype */
        if((error = CSEM_Micro_Document_GetItems(doc, types, &items))) {
            goto FINISH_GET_ITEMS;
        }
        /* get values which has specified itemprop */
        for(i = 0; i < CSEM_List_Size(items); i++) {
            int j = 0;
            CSEM_List *properties = NULL;
            CSEM_List *values = NULL, *valueTypes = NULL;
            CSEM_Item *item = CSEM_List_Get(items, i);

            if((error = CSEM_Micro_Item_GetNamedPropertes(item, itemprop, &properties))) {
                goto FINISH_NAMED_PROPERTIES;
            }
            if((error = CSEM_Micro_Properties_GetValues(properties, &values, &valueTypes))) {
                goto FINISH_NAMED_PROPERTIES;
            }
            /* show values */
            for(j = 0; j < CSEM_List_Size(values); j++) {
                int *valueType = CSEM_List_Get(valueTypes, j);
                if(*valueType == CSEM_MICRO_VALUE_TYPE_STR || *valueType == CSEM_MICRO_VALUE_TYPE_URL) {
                    printf("%s\n", (char *)CSEM_List_Get(values, j));
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
    puts("END");
    return error ? 1 : 0;
}
