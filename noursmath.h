#ifndef NOURSMATH_H
#define NOURSMATH_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/***************************** NOURS_MATH *************************/
/* Math library I made during the development of my game, Codename:Firesaga
* 3 Main modules:
*     q_math, quick math module
*     linalg, linear algebra utilities for n-dimensional matrices (like numpy)
*     pathfinding, utilities for games set on 2D a grid/matrix
* + Utility module DARR for dynamic arrays
*/

#ifndef DARR
#define DARR

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/********************** DARR: DYNAMIC ARRAYS FOR C99 v1.0 ******************/
// A DARR is an array with two additional elements: 
//   -> allocated length (len) at [-2] and number of active element (num) at [-1]

#define DARR_GROWTH_FACTOR 2
#define DARR_LEN_INDEX 2
#define DARR_NUM_INDEX 1

#define DARR_LEN(darr) (*((size_t *)darr - DARR_LEN_INDEX)) // allocated length
#define DARR_NUM(darr) (*((size_t *)darr - DARR_NUM_INDEX)) // number of active elements

// DARR_INIT: a DARR is an array with  size_t num at -1 and size_t len at -2,
#define DARR_INIT(darr, type, len)(type*)(((size_t* )malloc(sizeof(size_t)*DARR_LEN_INDEX + sizeof(type)*(len))) + DARR_LEN_INDEX);\
    DARR_LEN(darr) = len;\
    DARR_NUM(darr) = 0;\

// DARR_REALLOC: DARR internal. Not to be called directly by users.
#define DARR_REALLOC(darr, len) (void *)((size_t* )realloc(((size_t* )darr - DARR_LEN_INDEX), (sizeof(size_t)*DARR_LEN_INDEX + (sizeof(*darr))*(len))) + DARR_LEN_INDEX)

// DARR_GROW: increase array length by multiplying DARR_GROWTH_FACTOR
#define DARR_GROW(darr) do {\
    DARR_LEN(darr)*=DARR_GROWTH_FACTOR;\
    darr = DARR_REALLOC(darr, DARR_LEN(darr));\
} while(0)

// DARR_PUT: put elem on top of darr, at DARR_NUM
#define DARR_PUT(darr, elem) do {if ((DARR_NUM(darr) + 1) >= (DARR_LEN(darr))) {\
DARR_GROW(darr);\
}\
darr[DARR_NUM(darr)++] = elem;\
} while(0)

// DARR_POP: get top element of darr, and "remove" it by decrementing DARR_NUM
#define DARR_POP(darr) darr[--DARR_NUM(darr)]

// DARR_DEL_SCRAMBLE: delete elem by copying top element over it, and decrementing DARR_NUM
#define DARR_DEL_SCRAMBLE(darr, elem) do {\
    if (elem < DARR_NUM(darr)) {\
        memmove((darr + elem), (darr + --DARR_NUM(darr)), sizeof(*darr));\
    }\
} while(0)

// DARR_DEL: delete elem by moving all subsequent elements over
#define DARR_DEL(darr, elem) do {\
    if (elem < DARR_NUM(darr)) {\
        memmove((darr + elem), (darr + elem + 1), sizeof(*darr) * (--DARR_NUM(darr) - elem));\
    }\
} while(0)

// DARR_FREE: free whole darr
#define DARR_FREE(darr) do {free((((size_t* )darr) - DARR_LEN_INDEX));} while(0)

#endif /* DARR */

/******************************** CONSTANTS *********************************/

#ifndef TEMPLATE_TYPES_INT
#define TEMPLATE_TYPES_INT REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t)\
REGISTER_ENUM(bool)
#endif  /* TEMPLATE_TYPES_INT */

#ifndef TEMPLATE_TYPES_FLOAT
#define TEMPLATE_TYPES_FLOAT REGISTER_ENUM(float) \
REGISTER_ENUM(double)
#endif  /* TEMPLATE_TYPES_FLOAT */

#ifndef TEMPLATE_TYPES
#define TEMPLATE_TYPES REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t) \
REGISTER_ENUM(bool) \
REGISTER_ENUM(float) \
REGISTER_ENUM(double)
#endif  /* TEMPLATE_TYPES */

enum DIMENSIONS {
    ONE_D = 1,
    TWO_D = 2,
    THREE_D = 3,
};
#define NMATH_MINLEN 12

/******************************** STRUCTS ****************************/

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type {\
type right;\
type top;\
type left;\
type bottom;\
} nmath_sq_neighbors_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hex_neighbors_##type {\
type right;\
type top;\
type left;\
type bottom;\
type front;\
type behind;\
} nmath_hex_neighbors_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_point_##type {\
type x;\
type y;\
} nmath_point_##type##_default;
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_point_3D_##type {\
type x;\
type y;\
type z;\
} nmath_point_3D_##type##_default;
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hexpoint_##type {\
type x;\
type y;\
type z;\
} nmath_hexpoint_##type##_default;
TEMPLATE_TYPES
#undef REGISTER_ENUM

/******************************** UTILITIES **********************************/

#define REGISTER_ENUM(type) extern type nmath_inbounds_##type(type pos, type boundmin, type boundmax);
TEMPLATE_TYPES
#undef REGISTER_ENUM

/********************************* LINALG ************************************/

// linalg uses unraveled arrays as n-dimensional matrices
// col->x, row->y, depth->z
#define linalg_index_arr2D(row, col, col_len) (row * col_len + col)
#define linalg_index_arr3D(row, col, depth, row_len, col_len) (row * col_len * depth_len + col * depth_len + depth)

#define REGISTER_ENUM(type) extern type linalg_determinant_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_trace_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_isIn_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_1D_##type(type * list_1D, size_t list_len, type x);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_2D_##type(type * list_1D, size_t list_len, type x, type y);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_3D_##type(type * list_1D, size_t list_len, type x, type y, type z);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * linalg_where_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern void linalg_matrix_print_##type(type * array, size_t row_len, size_t col_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_any_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_all_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance);
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len, int8_t sign);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len);
TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_distance_manhattan_##type(struct nmath_point_##type start, struct nmath_point_##type end);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_distance_euclidian_##type(struct nmath_point_##type start, struct nmath_point_##type end);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

/********************** PATHFINDING *********************/

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Block_##type(type * costmap_pushpull, size_t row_len, size_t col_len, struct nmath_point_##type start);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Pushable_##type(type * assailablemap, size_t row_len, size_t col_len, type range[2], struct nmath_point_##type target);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Pullable_##type(type * assailablemap, size_t row_len, size_t col_len, type range[2], struct nmath_point_##type target);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_PushPullable_##type(struct nmath_sq_neighbors_##type direction_block, struct nmath_sq_neighbors_##type pushpullable, size_t row_len, size_t col_len, struct nmath_point_##type start, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Moveable_##type(type * costmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type move, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_##type(type * blockmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type sight, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Attack_##type(type * move_matrix, size_t row_len, size_t col_len, type move, type range[2], uint8_t mode_output, uint8_t mode_movetile);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Moveable_Hex_##type(type * costmap, size_t row_len, size_t depth_len, struct nmath_hexpoint_##type  start, type move, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_Hex_##type(type * blockmap, size_t depth_len, size_t col_len, struct nmath_hexpoint_##type  start, type sight, uint8_t mode);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Assailable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, type range[2], uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Pushable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Pullable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_unitGradient_##type(type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Path_##type(type * move_matrix, size_t row_len, size_t col_len, struct nmath_point_##type start, struct nmath_point_##type end, uint8_t mode_path);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Path_step2position_##type(type * step_list, size_t list_len, struct nmath_point_##type start);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Path_position2step_##type(type * position_list, size_t list_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#endif /* NOURSMATH_H */