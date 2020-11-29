#include <criterion/criterion.h>

#include "vec.h"

DEC_VEC(iptr_vec, int *)
DEF_VEC(iptr_vec, int *)

Test(vec, create_null_free_func)
{
	struct iptr_vec *v = iptr_vec_create(NULL);

	int *ptr = malloc(sizeof(int));
	*ptr = 12;

	iptr_vec_destroy(v);

	// If vec_destroy has ownership of *ptr, this would cause a double free and crash
	// under ASAN
	free(ptr);
}

struct s {
	int a;
	int b;
};

DEC_VEC(svec, struct s *)
DEF_VEC(svec, struct s *)

Test(vec, create_with_free_func)
{
	struct s *s_0 = malloc(sizeof(struct s));
	struct s *s_1 = malloc(sizeof(struct s));

	struct svec *v = svec_create(free);

	svec_push_back(v, s_0);
	svec_push_back(v, s_1);

	svec_destroy(v);

	// If the vector does not destroy the elements, this will cause a memory leak and
	// ASAN will notice it
}

DEC_VEC(vec, long)
DEF_VEC(vec, long)

Test(vec, create_push_and_get)
{
	struct vec *v = vec_create(NULL);

	cr_assert_not_null(v);

	long value = 4;

	vec_push_back(v, value);

	long a;
	vec_get(v, 0, &a);

	cr_assert_eq(a, 4, "a equals %ld", a);

	vec_destroy(v);
}

Test(vec, get_first)
{
	struct vec *v = vec_create(NULL);

	vec_push_back(v, 0);
	vec_push_back(v, 1);
	vec_push_back(v, 2);

	long res;
	vec_get(v, 0, &res);
	cr_assert_eq(res, 0);

	vec_destroy(v);
}

Test(vec, get_n_th)
{
	struct vec *v = vec_create(NULL);

	vec_push_back(v, 2);
	vec_push_back(v, 4);
	vec_push_back(v, 6);

	long res;

	vec_get(v, 1, &res);
	cr_assert_eq(res, 4);

	vec_get(v, 2, &res);
	cr_assert_eq(res, 6);

	vec_destroy(v);
}

Test(vec, set_first)
{
	struct vec *v = vec_create(NULL);

	vec_push_back(v, 0);
	vec_push_back(v, 1);
	vec_push_back(v, 2);

	vec_set(v, 0, 150);

	long res;

	vec_get(v, 0, &res);
	cr_assert_eq(res, 150);

	vec_get(v, 1, &res);
	cr_assert_eq(res, 1);

	vec_get(v, 2, &res);
	cr_assert_eq(res, 2);

	vec_destroy(v);
}

Test(vec, set_n_th)
{
	struct vec *v = vec_create(NULL);

	vec_push_back(v, 2);
	vec_push_back(v, 4);
	vec_push_back(v, 6);

	vec_set(v, 2, 150);

	long res;

	vec_get(v, 0, &res);
	cr_assert_eq(res, 2);

	vec_get(v, 1, &res);
	cr_assert_eq(res, 4);

	vec_get(v, 2, &res);
	cr_assert_eq(res, 150);

	vec_destroy(v);
}

DEC_VEC(ivec, int)
DEF_VEC(ivec, int)

Test(vec, push_back_pop_back)
{
	struct ivec *v = ivec_create(NULL);

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

Test(vec, push_back_realloc)
{
	struct vec *v = vec_create(NULL);

	for (size_t i = 0; i < VEC_DEFAULT_CAP * 32; i++)
		cr_assert_eq(vec_push_back(v, i), VEC_OK);

	long res;

	for (ssize_t i = VEC_DEFAULT_CAP * 32 - 1; i >= 0; i--) {
		cr_assert_eq(vec_pop_back(v, &res), VEC_OK);
                cr_assert_eq(res, i);
	}

	vec_destroy(v);
}

Test(vec, pop_back_non_existent)
{
	struct vec *v = vec_create(NULL);

	long value = 1;

	vec_push_back(v, value);

	long res;

	vec_pop_back(v, &res);
	cr_assert_eq(res, 1);

	cr_assert_eq(vec_pop_back(v, &res), VEC_NOT_EXIST);

	vec_destroy(v);
}
