#ifndef LINALG_H
#define LINALG_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define LINALG_TEMPLATE_TYPES_INT REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t)

#define LINALG_TEMPLATE_TYPES_FLOAT REGISTER_ENUM(float) \
REGISTER_ENUM(double)

#define LINALG_TEMPLATE_TYPES REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t) \
 REGISTER_ENUM(float) \
REGISTER_ENUM(double)

/**********************UTILITIES FOR MULTI_DIMENSIONAL ARRAYS******************/
// col->x, row->y, depth->z
#define arr2d_index(row, col, col_len) (row * col_len + col)
#define arr3d_index(row, col, depth, row_len, col_len) (row * col_len * row_len + col * row_len + depth)
enum Dimensions {
    ONE_D = 1,
    TWO_D = 2,
    THREE_D = 3,
};

// to do:
// -> trace of 2D matrix
// -> determinant of square matrix
// -> dot product
// -> cross product
// -> Diagonalization?
// -> Eigenvalue/Eigenvectors?
// -> Matrix multiplication

#define REGISTER_ENUM(type) extern type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool array_isIn_##type(type * array, type to_find, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool list_isIn_1D_##type(type * list_1D, size_t list_len, type x);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool list_isIn_2D_##type(type * list_1D, size_t list_len, type x, type y);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool list_isIn_3D_##type(type * list_1D, size_t list_len, type x, type y, type z);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * array_where_##type(type * array, type to_find, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern void matrix_print_##type(type * array, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool matrix_equal_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * matrix_plus_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len, int8_t sign);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * matrix_and_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * matrix_or_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * matrix_mask_##type(type * matrix, type * mask, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * matrix2list_##type(type * matrix, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool matrix_isIn_##type(type * matrix, type to_find, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * matrix_where_##type(type * matrix, type to_find, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#endif /* LINALG_H */
