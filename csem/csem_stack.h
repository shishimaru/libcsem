/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#ifndef CSEM_STACK_H
#define CSEM_STACK_H

#ifdef __cplusplus
#define CSEM_NS_C_BEGIN extern "C" {
#define CSEM_NS_C_END }
#else
#define CSEM_NS_C_BEGIN
#define CSEM_NS_C_END
#endif

CSEM_NS_C_BEGIN

#include <stddef.h>
#include "csem_common.h"

/** Stack. */
typedef struct CSEM_Stack CSEM_Stack;

CSEM_Stack *CSEM_Stack_Create(size_t initial_size);
void CSEM_Stack_Dispose(CSEM_Stack *stack, CSEM_Bool free);

CSEM_Error CSEM_Stack_Push(CSEM_Stack *stack, const void *data);
void *CSEM_Stack_Pop(CSEM_Stack *stack);
size_t CSEM_Stack_Size(const CSEM_Stack *stack);
void *CSEM_Stack_Top(const CSEM_Stack *stack);

CSEM_NS_C_END

#endif /* end of CSEM_STACK_H */
