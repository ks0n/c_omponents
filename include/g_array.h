#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

/**
 * Internal file to be used by generic containers
 */

#include <stddef.h>

typedef void (*g_array_free_function)(void *);

enum g_array_state {
	G_ARR_SIZE = -1,
	G_ARR_OK = 0,
};

/**
 * Generic array structure
 */
struct g_array {
	size_t cap;

	size_t elm_size;
	void *items;

	g_array_free_function free_fn;
};

/**
 * Create a new generic array
 *
 * @param elm_size Size of the elements to store
 * @param cap Capacity of the array
 * @param free_fn Function to use to destroy elements inside the array. Pass NULL to not
 *          destroy anything
 *
 * @return NULL on error, the generic array otherwise
 */
struct g_array *g_array_create(size_t elm_size, size_t cap, g_array_free_function free_fn);

/**
 * Release the memory used and owned by the array. If `free_fn` was not NULL when creating
 * the array, this will release the memory of each element as well.
 *
 * @param array Array to free
 */
void g_array_destroy(struct g_array *array);

/**
 * Get an element stored in the array at a certain index
 *
 * @param array Array to get the element from
 * @param index Index at which to get the element
 *
 * @return NULL on error, the element otherwise
 */
void *g_array_get(struct g_array *array, size_t index);

/**
 * Set an element at a certain position in the array
 *
 * @param array Array to set the element from
 * @param elm Element to add to the array
 * @param index Index at which to set the element
 *
 * @return G_ARR_OK on success, G_ARR_SIZE if the index is outside of the array's bounds
 */
int g_array_set(struct g_array *array, void *elm, size_t index);

#endif /* !GENERIC_ARRAY_H */
