#include <stdio.h>
#include <fcntl.h>
#include <CUnit/CUnit.h>
#include "csem/csem_builder.h"
#include "csem/csem_micro_tree.h"

void main_small_test_tree() {
    CSEM_Error error = CSEM_ERROR_NONE;

    int fd = -1;
    if(!(fd = open("./data/recursive-itemprop.html", O_RDONLY))) {
        CU_FAIL_FATAL("failed fopen");
        goto FINISH;
    }
    CSEM_Builder *builder = NULL;
    if((error = CSEM_Builder_Create(&builder))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }

    if((error = CSEM_Builder_Parse(builder, fd))) {
        CU_FAIL_FATAL("failed parse");
        goto FINISH;
    }
    CSEM_Document *doc = NULL;
    CSEM_Builder_GetDocument(builder, &doc);
FINISH:
    CSEM_Builder_Dispose(builder);
    CSEM_Document_Dispose(doc);
}
