#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <stdint.h>

#define VEC_DEFAULT_CAP 8

typedef void (*vec_free_function)(void *);

/**
 * Generic vector struct
 */
struct vec;

/**
 * Create a new empty vector given the size of its elements
 *
 * @param elm_size Size of each element contained in the array
 * @param elm_free Function to use when freeing each element. NULL if the vector should
 *        not own any element and therefore not free them
 *
 * @return NULL on error, the newly created vector on success
 */
struct vec *vec_create(size_t elm_size, vec_free_function elm_free);

/**
 * Destroy a previously created vector
 *
 * @param v Valid vector created with @vec_create
 */
void vec_destroy(struct vec *v);

void vec_push_back(struct vec *v, void *el);
void* vec_pop_back(struct vec *v);
void* vec_get(struct vec *v, size_t i);

size_t vec_size(struct vec *v);

#endif /* ! VEC_H */
