#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <sys/types.h>

#define VEC_DEFAULT_GROWTH 2
#define VEC_DEFAULT_CAP 8

/**
 * Generic vector implementation.
 *
 * In order to declare a generic vector, invoke the DEC_VEC(Vector Name, Stored Type)
 * macro. To define the "methods" of this vector, call DEF_VEC(Vector Name, Stored Type)
 *
 * An example implementation would be the following:
 *
 * ------ int-vec.h ------
 *  DEC_VEC(int_vec, int)
 * -----------------------
 *
 * ------ int-vec.c ------
 *  DEF_VEC(int_vec, int)
 * -----------------------
 *
 * Now, files that #include int-vec.h can use the `struct int_vec` type.
 *
 * In order to call the "methods" of this type, use the generic macros defined in
 * this file. To make a parallel with OOP, it's as if a generic class only had
 * static (in the OOP sense) functions, and that their first parameter was an instance
 * of the class. This is classic "OOP-like C".
 *
 * So, while you could do the following in C++:
 *
 * ```c++
 * auto v = vec_create<int>();
 *
 * v.push_back<int>(12);
 * ```
 *
 * You'd have to do this in C using this library:
 *
 * ```c
 * struct int_vec v = vec_create(int_vec)();
 *
 * vec_push_back(int_vec)(v, 12);
 * ```
 */

typedef void (*vec_free_function)(void *);

enum vector_state {
	VEC_NOT_EXIST = -2,
	VEC_MEM_ERR = -1,
	VEC_OK = 0,
};

/**
 * Get the number of elements contained inside a vector
 *
 * @param vector The vector to get the number of elements of.
 *
 * @warn The pointer must be a pointer to a valid vector type, declared and defined
 *       earlier in the code.
 *
 * @return The number of elements in the vector on success
 */
size_t vec_size(void *vector);

/**
 * Set the free function for a vector to use. If this function is not called, or
 * if the argument given to it is NULL, no entities will be free upon the vector's
 * release
 *
 * @param vector Vector properly initialize
 * @param free_fn Free function for the vector to use on the elements when releasing
 *        memory
 */
void vec_set_free_fn(void *vector, vec_free_function free_fn);

/**
 * Create a new empty vector given the size of its elements
 *
 * @return NULL on error, the newly created vector on success
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(ivec, int)
 * DEF_VEC(ivec, int)
 *
 * struct ivec *v = vec_create(ivec)();
 * // We now have a valid vector of integers
 * ```
 */
#define vec_create(T) T##_create

/**
 * Destroy a previously created vector
 *
 * @param v Valid vector created with @vec_create
 *
 * ```
 * // Create a vector of ints
 * DEC_VEC(ss_vec, some_struct)
 * DEF_VEC(ss_vec, some_struct)
 *
 * struct ss_vec *v = vec_create(ss_vec)();
 * vec_set_free_fn(v, some_struct_destroy_function);
 *
 * vec_push_back(ss_vec)(some_struct_instance_0);
 * vec_push_back(ss_vec)(some_struct_instance_1);
 * vec_push_back(ss_vec)(some_struct_instance_2);
 *
 * // Destroy the vector, as well as calling some_struct_destroy_function() on
 * // every item added to the vector
 * vec_destroy(ss_vec)(v);
 * ```
 */
#define vec_destroy(T) T##_destroy

/**
 * Append an element to the end of the vector
 *
 * @param v Vector to append an element to
 * @param el Element to add to the vector
 *
 * @return VEC_OK on success, VEC_MEM_ERR in case of memory error
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(any_vec, int)
 * DEF_VEC(any_vec, int)
 *
 * struct any_vec *v = vec_create(any_vec)();
 *
 * vec_push_back(any_vec)(12);
 * vec_push_back(any_vec)(16);
 * vec_push_back(any_vec)(20);
 *
 * // Vector `v` now contains 12, 16, 20 in that order, contiguously
 * ```
 */
#define vec_push_back(T) T##_push_back

/**
 * Remove the last element from the vector and return it
 *
 * @param v Vector to pop from
 * @param value Pointer in which to store the value
 *
 * @return The element on success, the default value otherwise
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(any_vec, int)
 * DEF_VEC(any_vec, int)
 *
 * struct any_vec *v = vec_create(any_vec)();
 *
 * int value;
 *
 * vec_push_back(any_vec)(50);
 * vec_pop_back(any_vec)(v, &value); // `value` is now 50
 * ```
 */
#define vec_pop_back(T) T##_pop_back

/**
 * Get an element from a vector at a given index
 *
 * @param v Vector to get the data from
 * @param index Index at which to get the data
 * @param value Pointer in which to store the value
 *
 * @return VEC_OK on success, VEC_NOT_EXIST otherwise
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(any_vec, int)
 * DEF_VEC(any_vec, int)
 *
 * struct any_vec *v = vec_create(any_vec)();
 *
 * vec_push_back(any_vec)(50);
 *
 * int value;
 * vec_get(any_vec)(v, 0, &value); // `value` is now 50, the return code is VEC_OK
 * vec_get(any_vec)(v, 9999, &value); // `value` is undefined, the return code is VEC_NOT_EXIST
 * ```
 */
#define vec_get(T) T##_get

/**
 * Set an element in a vector at a given index
 *
 * @param v Vector to set the data of
 * @param index Index at which to set the data
 * @param value Value to store in the vector
 *
 * @return VEC_OK on success, VEC_NOT_EXIST if
 *         the index is not present in the vector
 *
 * ```c
 * // Create a vector of ints
 * DEC_VEC(any_vec, int)
 * DEF_VEC(any_vec, int)
 *
 * struct any_vec *v = vec_create(any_vec)();
 *
 * vec_push_back(any_vec)(v, 100);
 * vec_push_back(any_vec)(v, 200);
 * vec_push_back(any_vec)(v, 300);
 *
 * vec_set(any_vec)(v, 0, 40); // vec_get(0) will now return 40 instead of 100
 * vec_set(any_vec)(v, 9999, 40); // VEC_NOT_EXIST
 * ```
 */
#define vec_set(T) T##_set

#include "hidden_vec.h"

#endif /* ! VEC_H */
