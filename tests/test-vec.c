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

Test(vec, create_set_and_get)
{
    struct vec *v = vec_create(sizeof(long), NULL);

    cr_assert_not_null(v);

    vec_push_back(v, (void *) 4);

    long a = (long) vec_get(v, 0);

    cr_assert_eq(a, 4, "a equals %d", a);

    vec_destroy(v);
}
