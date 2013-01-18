/* $Id$ */
#include <stdio.h>
#include <fcntl.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

#define INDENT(n) {int i=0;for(i=0;i<n;i++){printf("  ");}}
/**
 * Sample of microdata tree builder.
 */
void showProperties(CSEM_List *properties, int depth);
void showItem(CSEM_Item *item, int depth) {
    if(item) {
        char *itemid = CSEM_Micro_Item_GetId(item);
        int j = 0;
        CSEM_List *types = CSEM_Micro_Item_GetTypes(item);
        CSEM_List *refs = CSEM_Micro_Item_GetRefs(item);
        CSEM_List *properties = CSEM_Micro_Item_GetProperties(item);

        INDENT(depth);puts("ITEM {");
        if(itemid) {
            INDENT(depth);printf("@itemid=%s\n", itemid);
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
        char *id_value = CSEM_Micro_Id_GetId(id);
        CSEM_List *properties = CSEM_Micro_Id_GetProperties(id);

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
        CSEM_List *names = CSEM_Micro_Property_GetNames(property);
        CSEM_List *values = NULL, *types = NULL;
        if((error = CSEM_Micro_Property_GetValues(property, &values, &types))) {
            goto FINISH;
        }
        for(i = 0; names && i < CSEM_List_Size(names); i++) {
            INDENT(depth);printf("@itemprop=%s\n", (char *)CSEM_List_Get(names, i));
        }
        for(i = 0; values && i < CSEM_List_Size(values); i++) {
            int *type = CSEM_List_Get(types, i);

            if(*type == CSEM_MICRO_VALUE_TYPE_STR || *type == CSEM_MICRO_VALUE_TYPE_URL) {
                INDENT(depth);printf("\"%s\"\n", (char *)CSEM_List_Get(values, i));
            } else if(*type == CSEM_MICRO_VALUE_TYPE_ITEM) {
                CSEM_Item *item = CSEM_List_Get(values, i);
                showItem(item, depth + 1);
            } else if(*type == CSEM_MICRO_VALUE_TYPE_PROPERTY) {
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
    CSEM_Document *doc = NULL;
    char *filename = NULL;
    int fd = -1;

    /* setup input parameters */
    if(argc < 2) {
        error = CSEM_ERROR_PARAMETER;
        printf("./sample [filename]\n");
        goto FINISH;
    }
    filename = argv[1];

    /* init */
    if((error = CSEM_Builder_Create(&builder))) {
        goto FINISH;
    }
    /* start build process */
    if(!(fd = open(filename, O_RDONLY))) {
        fprintf(stderr, "failed to open %s\n", filename);
        goto FINISH;
    }
    if((error = CSEM_Builder_Parse(builder, fd, CSEM_TRUE, &doc))) {
        fprintf(stderr, "failed parsing %s\n", filename);
        goto FINISH;
    }
    {
        int i = 0;
        CSEM_List *nodes = CSEM_Document_GetChildren(doc);
        for(i = 0; i < CSEM_List_Size(nodes); i++) {
            CSEM_Node *node = CSEM_List_Get(nodes, i);
            if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_MICRO_ITEM) {
                CSEM_Item *item = CSEM_Node_GetObject(node);
                showItem(item, 0);
            } else if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_MICRO_ID) {
                CSEM_Id *id = CSEM_Node_GetObject(node);
                showId(id, 0);
            } else if(CSEM_Node_GetType(node) == CSEM_NODE_TYPE_MICRO_PROPERTY) {
                CSEM_Property *property = CSEM_Node_GetObject(node);
                showProperty(property, 0);
            }
        }
    }

FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
    puts("END");
    return error ? 1 : 0;
}
