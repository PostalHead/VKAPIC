#ifndef VEC_H
#define VEC_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_GROWTH_MULT 2

typedef struct {
    size_t count;
    size_t len;
    void* data;
} Vec;

Vec* vec_new(size_t count, size_t size) {
    size_t len = 0;
    if (count == 0) len = VEC_GROWTH_MULT;
    else len = count * VEC_GROWTH_MULT;

    Vec* vec = (Vec*)malloc(sizeof(Vec) + len * size);

    if (vec == NULL) {
        return NULL;
    }

    vec->count = count;
    vec->len = len;         
    
    vec->data = (void*)(vec + 1);

    return vec;
}

int vec_push(Vec** vec_var, void* value_ptr, size_t size) {
    Vec* vec = *vec_var;
    if (vec->count == vec->len) {
        size_t new_len = vec->len * VEC_GROWTH_MULT;
        Vec* new_vec = (Vec*)realloc(vec, sizeof(Vec) + new_len * size);

        if (new_vec == NULL) {
            return -1;
        }

        new_vec->len = new_len;
        new_vec->data = (void*)(new_vec + 1);
        *vec_var = new_vec; 
        vec = new_vec;
    }

    void* dest = (char*)(vec + 1) + (vec->count * size);
    memcpy(dest, value_ptr, size);
    vec->count++;

    return 0;
}

void* vec_get(Vec* vec, size_t index, size_t size) {
    if (vec->count <= index) {
        return NULL;
    }

    return (char*)(vec + 1) + index * size;
} 

void* vec_peek(Vec* vec, size_t size) {
    return vec_get(vec, vec->count - 1, size);
}

void* vec_pop(Vec* vec, size_t size) {
    void* element_ptr = vec_get(vec, vec->count - 1, size);

    if (element_ptr == NULL) {
        return NULL;
    }

    vec->count--;

    return element_ptr;
}

int vec_free(Vec* vec) {
    if (vec == NULL) {
        return -1;      
    }

    free(vec); 

    return 0;
}

#endif