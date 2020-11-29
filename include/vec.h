#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <sys/types.h>

#define VEC_DEFAULT_GROWTH 2
#define VEC_DEFAULT_CAP 8

typedef void (*vec_free_function)(void *);

enum vector_state {
	VEC_NOT_EXIST = -2,
	VEC_MEM_ERR = -1,
	VEC_OK = 0,
};

/**
 * Generic vector struct.
 *
 * The vector takes a POINTER to the data you want to store. Therefore, it has
 * to stay valid for the entierity of the vector's lifetime
 */
#define DEC_VEC(__VNAME, __TYPE)                                               \
	struct __VNAME;                                                        \
                                                                               \
	struct __VNAME *__VNAME##_create(vec_free_function elm_free);          \
	void __VNAME##_destroy(struct __VNAME *vector);                        \
	int __VNAME##_push_back(struct __VNAME *vector, __TYPE value);         \
	int __VNAME##_pop_back(struct __VNAME *vector, __TYPE *value);         \
	int __VNAME##_get(struct __VNAME *vector, size_t index,                \
			  __TYPE *value);                                      \
	int __VNAME##_set(struct __VNAME *vector, size_t index, __TYPE value);

void *__g_i_v_create(vec_free_function elm_free, size_t elm_size);
void __g_i_v_destroy(void *v);
int __g_i_v_extend_vec(void *v, size_t elm_size);

#define DEF_VEC(__VNAME, __TYPE)                                               \
	struct __VNAME {                                                       \
		size_t size;                                                   \
		size_t cap;                                                    \
		__TYPE *data;                                                  \
                                                                               \
		vec_free_function free_fn;                                     \
	};                                                                     \
                                                                               \
	struct __VNAME *__VNAME##_create(vec_free_function elm_free)           \
	{                                                                      \
		struct __VNAME *v = __g_i_v_create(elm_free, sizeof(__TYPE));  \
                                                                               \
		return v;                                                      \
	}                                                                      \
                                                                               \
	void __VNAME##_destroy(struct __VNAME *vector)                         \
	{                                                                      \
		__g_i_v_destroy(vector);                                       \
	}                                                                      \
                                                                               \
	int __VNAME##_push_back(struct __VNAME *v, __TYPE value)               \
	{                                                                      \
		if (v->size + 1 >= v->cap)                                     \
			if (__g_i_v_extend_vec(v, sizeof(__TYPE)) != VEC_OK)   \
				return VEC_MEM_ERR;                            \
                                                                               \
		v->data[v->size] = value;                                      \
		v->size++;                                                     \
                                                                               \
		return VEC_OK;                                                 \
	}                                                                      \
                                                                               \
	int __VNAME##_pop_back(struct __VNAME *v, __TYPE *value)               \
	{                                                                      \
		if (v->size == 0)                                              \
			return VEC_NOT_EXIST;                                  \
                                                                               \
		if (__VNAME##_get(v, v->size - 1, value) != VEC_OK)            \
			return VEC_NOT_EXIST;                                  \
		v->size--;                                                     \
                                                                               \
		return VEC_OK;                                                 \
	}                                                                      \
                                                                               \
	int __VNAME##_get(struct __VNAME *v, size_t index, __TYPE *value)      \
	{                                                                      \
		if (index >= v->size)                                          \
			return VEC_NOT_EXIST;                                  \
                                                                               \
		*value = v->data[index];                                       \
                                                                               \
		return VEC_OK;                                                 \
	}                                                                      \
                                                                               \
	int __VNAME##_set(struct __VNAME *v, size_t index, __TYPE value)       \
	{                                                                      \
		if (index >= v->size)                                          \
			return VEC_NOT_EXIST;                                  \
                                                                               \
		v->data[index] = value;                                        \
		v->size++;                                                     \
                                                                               \
		return VEC_OK;                                                 \
	}

// /**
//  * Create a new empty vector given the size of its elements
//  *
//  * @param elm_size Size of each element contained in the vector
//  * @param elm_free Function to use when freeing each element. NULL if the vector should
//  *        not own - meaning, should take care of their memory - any element
//  *        and therefore not free them
//  *
//  * @return NULL on error, the newly created vector on success
//  */
// struct vec *vec_create(size_t elm_size, vec_free_function elm_free);
//
// /**
//  * Destroy a previously created vector
//  *
//  * @param v Valid vector created with @vec_create
//  */
// void vec_destroy(struct vec *v);
//
// /**
//  * Append an element to the end of the vector
//  *
//  * @param v Vector to append an element to
//  * @param el Element to add to the vector
//  *
//  * @return VEC_OK on success, VEC_MEM_ERR in case of memory error
//  */
// int vec_push_back(struct vec *v, void *el);
//
// /**
//  * Remove the last element from the vector and return it
//  *
//  * @param v Vector to pop from
//  *
//  * @return The element on success, NULL otherwise
//  */
// void* vec_pop_back(struct vec *v);
//
// /**
//  * Get an element from a vector at a given index
//  *
//  * @return The element on success, NULL otherwise
//  */
// void* vec_get(struct vec *v, size_t i);
//
// /**
//  * Set an element in a vector at a given index
//  *
//  * @return VEC_OK on success, VEC_MEM_ERR on allocation failure
//  */
// int vec_set(struct vec *v, void *elm, size_t i);
//
// /**
//  * Get the number of elements contained inside a vector
//  *
//  * @param v The vector to get the number of elements of.
//  *
//  * @warn The pointer must be a pointer to a valid vector type, declared and defined
//  *       earlier in the code.
//  *
//  * @return The number of elements in the vector on success
//  */
// size_t vec_size(void *v);

#endif /* ! VEC_H */
