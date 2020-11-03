#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <sys/types.h>

#define VEC_DEFAULT_CAP 8

typedef void (*vec_free_function)(void *);

enum vector_state {
    VEC_NULL,
    VEC_MEM_ERR,
    VEC_OK = 0,
};

/**
 * Generic vector struct
 */
struct vec;

/**
 * Create a new empty vector given the size of its elements
 *
 * @param elm_size Size of each element contained in the vector
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

/**
 * Append an element to the end of the vector
 *
 * @param v Vector to append an element to
 * @param el Element to add to the vector
 *
 * @return VEC_OK on success, VEC_MEM_ERR in case of memory error
 */
int vec_push_back(struct vec *v, void *el);

/**
 * Remove the last element from the vector and return it
 *
 * @param v Vector to pop from
 *
 * @return The element on success, NULL otherwise
 */
void* vec_pop_back(struct vec *v);

/**
 * Get an element from a vector at a given index
 *
 * @return The element on success, NULL otherwise
 */
void* vec_get(struct vec *v, size_t i);

/**
 * Get the number of elements contained inside a vector
 *
 * @param v The vector to get the number of elements of
 *
 * @return The number of elements in the vector on success, VEC_NULL otherwise
 */
ssize_t vec_size(struct vec *v);

#endif /* ! VEC_H */
