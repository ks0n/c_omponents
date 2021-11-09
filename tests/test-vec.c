#include <criterion/criterion.h>

#include "vec.h"

DEC_VEC(iptr_vec, int *)
DEF_VEC(iptr_vec, int *)

Test(lvec, create_null_free_func)
{
	struct iptr_vec *v = iptr_vec_create();

	int *ptr = malloc(sizeof(int));
	*ptr = 12;

	iptr_vec_destroy(v);

	// If lvec_destroy has ownership of *ptr, this would cause a double free and crash
	// under ASAN
	free(ptr);
}

struct s {
	int a;
	int b;
};

DEC_VEC(svec, struct s *)
DEF_VEC(svec, struct s *)

Test(lvec, create_with_free_func)
{
	struct s *s_0 = malloc(sizeof(struct s));
	struct s *s_1 = malloc(sizeof(struct s));

	struct svec *v = svec_create();
	vec_set_free_fn(v, free);

	svec_push_back(v, s_0);
	svec_push_back(v, s_1);

	svec_destroy(v);

	// If the vector does not destroy the elements, this will cause a memory leak and
	// ASAN will notice it
}

DEC_VEC(lvec, long)
DEF_VEC(lvec, long)

Test(lvec, create_push_and_get)
{
	struct lvec *v = lvec_create();

	cr_assert_not_null(v);

	long value = 4;

	lvec_push_back(v, value);

	long a;
	lvec_get(v, 0, &a);

	cr_assert_eq(a, 4, "a equals %ld", a);

	lvec_destroy(v);
}

Test(lvec, get_first)
{
	struct lvec *v = lvec_create();

	lvec_push_back(v, 0);
	lvec_push_back(v, 1);
	lvec_push_back(v, 2);

	long res;
	lvec_get(v, 0, &res);
	cr_assert_eq(res, 0);

	lvec_destroy(v);
}

Test(lvec, get_n_th)
{
	struct lvec *v = lvec_create();

	lvec_push_back(v, 2);
	lvec_push_back(v, 4);
	lvec_push_back(v, 6);

	long res;

	lvec_get(v, 1, &res);
	cr_assert_eq(res, 4);

	lvec_get(v, 2, &res);
	cr_assert_eq(res, 6);

	lvec_destroy(v);
}

Test(lvec, set_first)
{
	struct lvec *v = lvec_create();

	lvec_push_back(v, 0);
	lvec_push_back(v, 1);
	lvec_push_back(v, 2);

	lvec_set(v, 0, 150);

	long res;

	lvec_get(v, 0, &res);
	cr_assert_eq(res, 150);

	lvec_get(v, 1, &res);
	cr_assert_eq(res, 1);

	lvec_get(v, 2, &res);
	cr_assert_eq(res, 2);

	lvec_destroy(v);
}

Test(lvec, set_n_th)
{
	struct lvec *v = lvec_create();

	lvec_push_back(v, 2);
	lvec_push_back(v, 4);
	lvec_push_back(v, 6);

	lvec_set(v, 2, 150);

	long res;

	lvec_get(v, 0, &res);
	cr_assert_eq(res, 2);

	lvec_get(v, 1, &res);
	cr_assert_eq(res, 4);

	lvec_get(v, 2, &res);
	cr_assert_eq(res, 150);

	lvec_destroy(v);
}

DEC_VEC(ivec, int)
DEF_VEC(ivec, int)

Test(lvec, push_back_pop_back)
{
	struct ivec *v = ivec_create();

	ivec_push_back(v, 1);
	ivec_push_back(v, 2);
	ivec_push_back(v, 3);

	int res;
	ivec_pop_back(v, &res);
	cr_assert_eq(res, 3);

	ivec_pop_back(v, &res);
	cr_assert_eq(res, 2);

	ivec_destroy(v);
}

Test(lvec, push_back_realloc)
{
	struct lvec *v = lvec_create();

	for (size_t i = 0; i < VEC_DEFAULT_CAP * 32; i++)
		cr_assert_eq(lvec_push_back(v, i), VEC_OK);

	long res;

	for (ssize_t i = VEC_DEFAULT_CAP * 32 - 1; i >= 0; i--) {
		cr_assert_eq(lvec_pop_back(v, &res), VEC_OK);
		cr_assert_eq(res, i);
	}

	lvec_destroy(v);
}

Test(lvec, pop_back_non_existent)
{
	struct lvec *v = lvec_create();

	long value = 1;

	lvec_push_back(v, value);

	long res;

	lvec_pop_back(v, &res);
	cr_assert_eq(res, 1);

	cr_assert_eq(lvec_pop_back(v, &res), VEC_NOT_EXIST);

	lvec_destroy(v);
}

Test(lvec, generic_macros)
{
	struct lvec *v = vec_create(lvec)();

	vec_push_back(lvec)(v, 10);
	vec_push_back(lvec)(v, 11);
	vec_push_back(lvec)(v, 12);

	long value;

	vec_pop_back(lvec)(v, &value);

	cr_assert_eq(value, 12);
}
