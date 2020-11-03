#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "vec.h"

struct vec {
	size_t size;
	size_t cap;

	vec_free_function elm_free;
	size_t elm_size;
	void *items;
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

	v->elm_size = elm_size;
	v->elm_free = elm_free;
	v->size = 0;
	v->cap = VEC_DEFAULT_CAP;

	v->items = malloc(v->cap * elm_size);
	if (v->items == NULL)
		return NULL;

	return v;
}

static char *get_items_offset(struct vec *v, size_t i)
{
	char *elms = v->items;
	elms += i * v->elm_size;

	return elms;
}

static void *vec_get_inner(struct vec *v, size_t i)
{
	char *elms = get_items_offset(v, i);

	void *value;
	memcpy(&value, elms, v->elm_size);

	return value;
}

void vec_set_inner(struct vec *v, size_t i, void *value)
{
	char *elms = get_items_offset(v, i);

	memcpy(elms, &value, v->elm_size);
}

void vec_destroy(struct vec *v)
{
	for (size_t i = 0; i < v->size; i++)
		if (v->elm_free)
			v->elm_free(vec_get(v, i));

	free(v->items);
	free(v);
}

static int extend_vec(struct vec *v)
{
	v->cap *= 2;
	v->items = realloc(v->items, v->cap * v->elm_size);

	if (!v->items) {
		free(v);
		return VEC_MEM_ERR;
	}

	return VEC_OK;
}

int vec_push_back(struct vec *v, void *el)
{
	if (v->size + 1 >= v->cap)
		if (extend_vec(v) != VEC_OK)
			return VEC_MEM_ERR;

	vec_set_inner(v, vec_size(v), el);

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

	return (void *)vec_get_inner(v, i);
}
