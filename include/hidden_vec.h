#pragma once

/**
 * This file should not be considered part of the generic API. It is subject
 * to changes without notice, and should only be used by the library's
 * developers.
 */

#include "vec.h"

#define DEC_VEC(__VNAME, __TYPE)                                               \
	struct __VNAME;                                                        \
                                                                               \
	/**
         * Create a new empty vector given the size of its elements
         *
         * @param elm_size Size of each element contained in the vector
         * @param elm_free Function to use when freeing each element. NULL if the vector should
         *        not own - meaning, should take care of their memory - any element
         *        and therefore not free them
         *
         * @return NULL on error, the newly created vector on success
         */                                                                    \
	struct __VNAME *__VNAME##_create(vec_free_function elm_free);          \
                                                                               \
	/**
          * Destroy a previously created vector
          *
          * @param v Valid vector created with @vec_create
          */                                                                    \
	void __VNAME##_destroy(struct __VNAME *vector);                        \
                                                                               \
	/**
         * Append an element to the end of the vector
         *
         * @param v Vector to append an element to
         * @param el Element to add to the vector
         *
         * @return VEC_OK on success, VEC_MEM_ERR in case of memory error
         */                                                                    \
	int __VNAME##_push_back(struct __VNAME *vector, __TYPE value);         \
                                                                               \
	/**
         * Remove the last element from the vector and return it
         *
         * @param v Vector to pop from
         *
         * @return The element on success, NULL otherwise
         */                                                                    \
	int __VNAME##_pop_back(struct __VNAME *vector, __TYPE *value);         \
                                                                               \
	/**
         * Get an element from a vector at a given index
         *
         * @return The element on success, NULL otherwise
         */                                                                    \
	int __VNAME##_get(struct __VNAME *vector, size_t index,                \
			  __TYPE *value);                                      \
                                                                               \
	/**
         * Set an element in a vector at a given index
         *
         * @return VEC_OK on success, VEC_MEM_ERR on allocation failure
         */                                                                    \
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
