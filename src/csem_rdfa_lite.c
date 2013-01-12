/* $Id$ */
#include "csem/csem_rdfa_lite.h"
#include "csem_utils.h"
#include "csem_types.h"

CSEM_Error CSEM_RDFaLite_CreateHandler(CSEM_RDFaLite_Handlers **handler) {
    CSEM_Error error = CSEM_ERROR_NONE;
    CSEM_RDFaLite_Handlers *tmp_handler = NULL;

    if(!(tmp_handler = CSEM_Calloc(1, sizeof(CSEM_RDFaLite_Handlers)))) {
        error = CSEM_ERROR_MEMORY;
        goto ERROR;
    }
    /* result */
    *handler = tmp_handler;
    return error;
ERROR:
    CSEM_RDFaLite_DisposeHandler(tmp_handler);
    return error;
}
void CSEM_RDFaLite_DisposeHandler(CSEM_RDFaLite_Handlers *handler) {
    if(handler) {
        CSEM_Free(handler);
    }
}
