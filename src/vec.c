#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vec.h"
#include "g_array.h"

struct vec {
	size_t size;
	size_t cap;

	struct g_array *array;
};

inline size_t vec_size(struct vec *v)
{
	return v->size;
}

struct vec *vec_create(size_t elm_size, vec_free_function elm_free)
{
	if (!elm_size)
		return NULL;

	struct vec *v = malloc(sizeof(struct vec));
	if (v == NULL)
		return NULL;

	struct g_array *a = g_array_create(elm_size, VEC_DEFAULT_CAP, elm_free);
	if (!a) {
		free(v);
		return NULL;
	}

	v->array = a;
	v->size = 0;
	v->cap = VEC_DEFAULT_CAP;

	return v;
}

void vec_destroy(struct vec *v)
{
	g_array_destroy(v->array);
	free(v);
}

static int extend_vec(struct vec *v)
{
	v->cap *= 2;

	struct g_array *old_array = v->array;
	struct g_array *new_array =
		g_array_create(old_array->elm_size, v->cap, old_array->free_fn);
	if (!new_array) {
		return VEC_MEM_ERR;
	}

	memcpy(new_array->items, old_array->items,
	       old_array->cap * old_array->elm_size);

	v->array = new_array;

	return VEC_OK;
}

int vec_push_back(struct vec *v, void *el)
{
	if (v->size + 1 >= v->cap)
		if (extend_vec(v) != VEC_OK)
			return VEC_MEM_ERR;

	/* Cannot fail since we checked the size just before */
	g_array_set(v->array, el, vec_size(v));

	v->size++;

	return VEC_OK;
}

void *vec_pop_back(struct vec *v)
{
	if (vec_size(v) == 0)
		return NULL;

	void *last = vec_get(v, vec_size(v) - 1);
	v->size--;

	return last;
}

void *vec_get(struct vec *v, size_t i)
{
	if (i >= v->size)
		return NULL;

	return g_array_get(v->array, i);
}
