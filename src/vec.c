#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vec.h"
#include "g_array.h"

struct __generic_inner_vec__ {
	size_t size;
	size_t cap;
	void **data;

	vec_free_function free_fn;
};

static inline size_t vec_size(void *vector)
{
	struct __generic_inner_vec__ *v = vector;
	return v->size;
}

void *__g_i_v_create(vec_free_function elm_free, size_t elm_size)
{
	struct __generic_inner_vec__ *v =
		malloc(sizeof(struct __generic_inner_vec__));
	if (v == NULL)
		return NULL;

	void *array = malloc(elm_size * VEC_DEFAULT_CAP);
	if (!array) {
		free(v);
		return NULL;
	}

	v->data = array;
	v->size = 0;
	v->cap = VEC_DEFAULT_CAP;
	v->free_fn = elm_free;

	return v;
}

void __g_i_v_destroy(void *v_void)
{
	struct __generic_inner_vec__ *v = v_void;
	if (v->free_fn)
		for (size_t i = 0; i < vec_size(v); i++)
			v->free_fn(v->data[i]);

	free(v->data);
	free(v);
}

int __g_i_v_extend_vec(void *v_void, size_t elm_size)
{
	struct __generic_inner_vec__ *v = v_void;
	v->cap *= VEC_DEFAULT_GROWTH;

	v->data = realloc(v->data, v->cap * elm_size);

	if (!v->data)
		return VEC_MEM_ERR;

	return VEC_OK;
}
