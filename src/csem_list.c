/*
 * $Id$
 * See Copyright for the status of this software.
 * uchida@w3.org
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csem/csem_list.h"
#include "csem_utils.h"

struct CSEM_List {
    size_t initial_size;
    size_t used_size;
    size_t allocated_size;
    void **data;
};

CSEM_List *CSEM_List_Create(size_t initial_size) {
    CSEM_List *list = NULL;

    if(!(list = CSEM_Malloc(sizeof(CSEM_List)))) {
        goto ERROR;
    }
    if(!(list -> data = CSEM_Calloc(initial_size, sizeof(void *)))) {
        goto ERROR;
    }

    list -> initial_size = initial_size;
    list -> used_size = 0;
    list -> allocated_size = initial_size;
    return list;
ERROR:
    CSEM_List_Dispose(list, CSEM_FALSE);
    return list;
}
void CSEM_List_Dispose(CSEM_List *list, CSEM_Bool freeData) {
    if(list) {
        CSEM_List_Clear(list, freeData);
        if(list -> data) {
            CSEM_Free(list -> data);
        }
        CSEM_Free(list);
    }
}
CSEM_Error CSEM_List_Add(CSEM_List *list, const void *data) {
    if(list->used_size >= list->allocated_size) {
        void *tmp = NULL;
        if(!(tmp = realloc(list->data, sizeof(void *) * (list->allocated_size + list->initial_size)))) {
            return CSEM_ERROR_MEMORY;
        }
        list->data = tmp;
        list->allocated_size = list->allocated_size + list->initial_size;
    }
    list-> data[list->used_size++] = (void *)data;
    return CSEM_ERROR_NONE;
}
CSEM_Bool CSEM_List_Contains(const CSEM_List *list, const void *data) {
    int index = CSEM_List_IndexOf(list, data);
    return index != -1;
}
int CSEM_List_IndexOf(const CSEM_List *list, const void *data) {
    int i = 0;
    FOR(i, 0, list -> used_size) {
        if(list->data[i] == data) {
            return i;
        }
    }
    return -1;
}
size_t CSEM_List_Size(const CSEM_List *list) {
    return list -> used_size;
}
void *CSEM_List_Get(const CSEM_List *list, int index) {
    if(index < 0 || index >= CSEM_List_Size(list)) {
        return NULL;
    }
    return list -> data[index];
}
CSEM_Bool CSEM_List_IsEmpty(const CSEM_List *list) {
    return list -> used_size == 0;
}
void *CSEM_List_Remove(CSEM_List *list, int index) {
    void *removed_data = NULL;
    if(index >= 0 && index < CSEM_List_Size(list)) {
        removed_data = list -> data[index];
        memmove(list->data + index, list->data + (index+1), sizeof(void *) * (list->used_size - (index+1)));
        list -> used_size--;
    }
    return removed_data;
}
void *CSEM_List_Replace(CSEM_List *list, int index, const void *data) {
    void *old = NULL;
    if(index >= 0 && index < CSEM_List_Size(list)) {
        old = list->data[index];
        list->data[index] = (void *)data;
    }
    return old;
}
void CSEM_List_Clear(CSEM_List *list, CSEM_Bool freeData) {
    if(freeData) {
        int i, len;
        for(i = 0, len = CSEM_List_Size(list); i < len; i++) {
            void *data = CSEM_List_Get(list, i);
            CSEM_Free(data);
        }
    }

    list -> used_size = 0;
    memset(list->data, 0, sizeof(void *) * list->used_size);
}
