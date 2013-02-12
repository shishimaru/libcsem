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

/**
 * Create a stack instance.
 * @param initial_size [in]initial size of the stack
 * @return a newly created stack
 */
CSEM_Stack *CSEM_Stack_Create(size_t initial_size);

/**
 * Dispose the stack instance.
 * @param stack [in]stack instance to be freed
 * @param free  [in]a flag to free the elements or not
 */
void CSEM_Stack_Dispose(CSEM_Stack *stack, CSEM_Bool free);

/**
 * Push new data into the stack.
 * @param stack [in]stack instance
 * @param data  [in]new data to be pushed
 * @return error
 */
CSEM_Error CSEM_Stack_Push(CSEM_Stack *stack, const void *data);

/**
 * Pop top data from the stack.
 * @param stack [in]stack instance
 * @return popped data
 */
void *CSEM_Stack_Pop(CSEM_Stack *stack);

/**
 * Get a length of the stack.
 * @param stack [in]stack instance
 * @return length
 */
size_t CSEM_Stack_Size(const CSEM_Stack *stack);

/**
 * Get top data in the stack.
 * @param stack [in]stack instance
 * @return top
 */
void *CSEM_Stack_Top(const CSEM_Stack *stack);

CSEM_NS_C_END

#endif /* end of CSEM_STACK_H */
