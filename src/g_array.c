#include <stdlib.h>
#include <string.h>

#include "g_array.h"

struct g_array *g_array_create(size_t elm_size, size_t cap,
			       g_array_free_function free_fn)
{
	struct g_array *arr = malloc(sizeof(struct g_array));
	if (!arr)
		return NULL;

	arr->elm_size = elm_size;
	arr->free_fn = free_fn;
	arr->cap = cap;

	arr->items = calloc(arr->cap, elm_size);
	if (arr->items == NULL) {
		free(arr);
		return NULL;
	}

	return arr;
}

void g_array_destroy(struct g_array *array)
{
	if (array->free_fn)
		for (size_t i = 0; i < array->cap; i++) {
			void *elm = g_array_get(array, i);
			if (elm)
				array->free_fn(elm);
		}

	free(array->items);
	free(array);
}

static char *get_items_offset(struct g_array *arr, size_t i)
{
	char *elms = arr->items;
	elms += i * arr->elm_size;

	return elms;
}

void *g_array_get(struct g_array *array, size_t index)
{
	if (index > array->cap)
		return NULL;

	char *elms = get_items_offset(array, index);

	void *value;
	memcpy(&value, elms, array->elm_size);

	return value;
}

int g_array_set(struct g_array *array, void *elm, size_t index)
{
	if (index > array->cap)
		return G_ARR_SIZE;

	char *elms = get_items_offset(array, index);

	memcpy(elms, &elm, array->elm_size);

	return G_ARR_OK;
}
