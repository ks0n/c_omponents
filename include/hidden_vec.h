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
	struct __VNAME *__VNAME##_create(void);                                \
	void *__VNAME##_set_free_fn(vec_free_function free_fn);                \
	void __VNAME##_destroy(struct __VNAME *vector);                        \
	int __VNAME##_push_back(struct __VNAME *vector, __TYPE value);         \
	int __VNAME##_pop_back(struct __VNAME *vector, __TYPE *value);         \
	int __VNAME##_get(struct __VNAME *vector, size_t index,                \
			  __TYPE *value);                                      \
	int __VNAME##_set(struct __VNAME *vector, size_t index, __TYPE value);

void *__g_i_v_create(size_t elm_size);
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
	struct __VNAME *__VNAME##_create(void)                                 \
	{                                                                      \
		return __g_i_v_create(sizeof(__TYPE));                         \
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
