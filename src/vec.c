#include <stdlib.h>
#include <stdint.h>

#include "vec.h"

struct vec {
	size_t size;
	size_t cap;

	vec_free_function elm_free;
	size_t elm_size;
	void *items;
};

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

static uintptr_t vec_get_inner(struct vec *v, size_t i)
{
	char *elms = v->items;

	return (uintptr_t)(elms + i * v->elm_size);
}

void vec_set_inner(struct vec *v, size_t i, uintptr_t value)
{
	char *elms = v->items;

	uintptr_t *offset = (void *)(elms + i * v->elm_size);

	*offset = value;
}

void vec_destroy(struct vec *v)
{
	for (size_t i = 0; i < v->size; i++)
		if (v->elm_free)
			v->elm_free(vec_get(v, i));

	free(v->items);
	free(v);
}

static void extend_vec(struct vec *v)
{
	v->cap *= 2;
	v->items = realloc(v->items, v->cap * v->elm_size);

    // FIXME: Check for NULL
}

void vec_push_back(struct vec *v, void *el)
{
	if (v->size + 1 >= v->cap)
		extend_vec(v);

	vec_set_inner(v, vec_size(v), (uintptr_t)el);

	v->size++;
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

inline size_t vec_size(struct vec *v)
{
	return v->size;
}
