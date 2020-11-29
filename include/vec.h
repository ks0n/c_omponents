#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <sys/types.h>

#define VEC_DEFAULT_GROWTH 2
#define VEC_DEFAULT_CAP 8

/**
 * Use DEC_VEC(Name, Type) to declare a new vector type. Call DEF_VEC(Name, Type) in
 * order to define the functions for this vector.
 *
 * FIXME: Add WAYYYYYY more documentation
 */

typedef void (*vec_free_function)(void *);

enum vector_state {
	VEC_NOT_EXIST = -2,
	VEC_MEM_ERR = -1,
	VEC_OK = 0,
};

/**
 * Get the number of elements contained inside a vector
 *
 * @param vector The vector to get the number of elements of.
 *
 * @warn The pointer must be a pointer to a valid vector type, declared and defined
 *       earlier in the code.
 *
 * @return The number of elements in the vector on success
 */
size_t vec_size(void *vector);

/**
 * Set the free function for a vector to use. If this function is not called, or
 * if the argument given to it is NULL, no entities will be free upon the vector's
 * release
 *
 * @param vector Vector properly initialize
 * @param free_fn Free function for the vector to use on the elements when releasing
 *        memory
 */
void vec_set_free_fn(void *vector, vec_free_function free_fn);

/**
 * Create a new empty vector given the size of its elements
 *
 * @return NULL on error, the newly created vector on success
 */
#define vec_create(T) T##_create

/**
 * Destroy a previously created vector
 *
 * @param v Valid vector created with @vec_create
 */
#define vec_destroy(T) T##_destroy

/**
 * Append an element to the end of the vector
 *
 * @param v Vector to append an element to
 * @param el Element to add to the vector
 *
 * @return VEC_OK on success, VEC_MEM_ERR in case of memory error
 */
#define vec_push_back(T) T##_push_back

/**
 * Remove the last element from the vector and return it
 *
 * @param v Vector to pop from
 *
 * @return The element on success, the default value otherwise
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(any_vec, int)
 * DEF_VEC(any_vec, int)
 *
 * struct any_vec *v = vec_create(any_vec)();
 *
 * vec_push_back(any_vec)(50);
 * vec_pop_back(any_vec)(); // We get 50
 * ```
 */
#define vec_pop_back(T) T##_pop_back

/**
 * Get an element from a vector at a given index
 *
 * @return The element on success, The default value otherwise
 */
#define vec_get(T) T##_get

/**
 * Set an element in a vector at a given index
 *
 * @return VEC_OK on success, VEC_MEM_ERR on allocation failure
 */
#define vec_set(T) T##_set

#include "hidden_vec.h"

#endif /* ! VEC_H */
