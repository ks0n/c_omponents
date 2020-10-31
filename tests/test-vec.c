#include <criterion/criterion.h>

#include "vec.h"

Test(vec, create_null_empty_elm)
{
	struct vec *v = vec_create(0, NULL);

	cr_assert_null(v);
}

Test(vec, create_null_ff)
{
	struct vec *v = vec_create(sizeof(int *), NULL);

	int *ptr = malloc(sizeof(int));
	*ptr = 12;

	vec_destroy(v);

	// If vec_destroy has ownership of *ptr, this would cause a double free and crash
	// under ASAN
	free(ptr);
}

Test(vec, create_with_ff)
{
	struct s {
		int a;
		int b;
	};

	struct s *s_0 = malloc(sizeof(struct s));
	struct s *s_1 = malloc(sizeof(struct s));

	struct vec *v = vec_create(sizeof(struct s *), free);

	vec_push_back(v, s_0);
	vec_push_back(v, s_1);

	vec_destroy(v);

	// If the vector does not destroy the elements, this will cause a memory leak and
	// ASAN will notice it
}

Test(vec, create_set_and_get)
{
	struct vec *v = vec_create(sizeof(long), NULL);

	cr_assert_not_null(v);

	vec_push_back(v, (void *)4);

	long a = (long)vec_get(v, 0);

	cr_assert_eq(a, 4, "a equals %ld", a);

	vec_destroy(v);
}

Test(vec, get_first)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    vec_push_back(v, (void *) 0);
    vec_push_back(v, (void *) 1);
    vec_push_back(v, (void *) 2);

    cr_assert_eq((long) vec_get(v, 0), 0);

    vec_destroy(v);
}

Test(vec, get_n_th)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    vec_push_back(v, (void *) 2);
    vec_push_back(v, (void *) 4);
    vec_push_back(v, (void *) 6);

    cr_assert_eq((long) vec_get(v, 1), 4);
    cr_assert_eq((long) vec_get(v, 2), 6);

    vec_destroy(v);
}

Test(vec, push_back_pop_back)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    vec_push_back(v, (void *) 1);
    vec_push_back(v, (void *) 2);
    vec_push_back(v, (void *) 3);

    cr_assert_eq((long) vec_pop_back(v), 3);
    cr_assert_eq((long) vec_pop_back(v), 2);

    vec_destroy(v);
}

Test(vec, push_back_realloc)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    for (size_t i = 0; i < 256; i++)
        vec_push_back(v, (void *) i);

    for (ssize_t i = 255; i >= 0; i--)
        cr_assert_eq((long) vec_pop_back(v), i);

    vec_destroy(v);
}

Test(vec, pop_back_non_existent)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    vec_push_back(v, (void *) 1);

    cr_assert_eq((long) vec_pop_back(v), 1);
    cr_assert_null(vec_pop_back(v));

    vec_destroy(v);
}

Test(vec, size_empty)
{
    cr_assert_eq(vec_size(NULL), VEC_NULL);
}
