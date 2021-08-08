#ifndef NOURSMATH_H
#define NOURSMATH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

/******************************* NOURS_MATH v0.2 *****************************/
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

#ifndef DTAB
#define DTAB

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/********************** DTAB: DYNAMIC HASH TABLE FOR C99 v1.0 ******************/
typedef uint64_t dtab_hash_t;
typedef unsigned char dtab_byte_t;

extern uint64_t dtab_hash_djb2(const char * str); // slightly faster
extern uint64_t dtab_hash_sdbm(const char * str);
#define DTAB_HASH(name) dtab_hash_djb2(name)

#define DTAB_LEN_INIT 16
#define DTAB_NUM_INIT 1
#define DTAB_NULL 0
#define DTAB_GROWTH_FACTOR 2

struct dtab {
    size_t bytesize;
    size_t len; /* allocated length */
    size_t num; /* number of active elements (num < len) */
    dtab_hash_t * keys;
    void * values;
};

extern void * dtab_get(struct dtab * dtab_ptr, dtab_hash_t in_hash);
extern void dtab_add(struct dtab * dtab_ptr, void * value, dtab_hash_t in_hash);
extern size_t dtab_found(struct dtab * dtab_ptr, dtab_hash_t in_hash);
extern void dtab_del(struct dtab * dtab_ptr, dtab_hash_t in_hash);
extern void dtab_del_scramble(struct dtab * dtab_ptr, dtab_hash_t in_hash);

#define DTAB_STRINGIFY(name) #name

#define DTAB_INIT(dtab_ptr, type) dtab_ptr = malloc(sizeof(*dtab_ptr));\
dtab_ptr->len = DTAB_LEN_INIT;\
dtab_ptr->num = DTAB_NUM_INIT;\
dtab_ptr->values = calloc(DTAB_LEN_INIT, sizeof(type));\
dtab_ptr->keys = malloc(sizeof(*dtab_ptr->keys) * (DTAB_LEN_INIT));\
dtab_ptr->keys[DTAB_NULL] = DTAB_NULL;\
dtab_ptr->bytesize = sizeof(type);
#define DTAB_GROW(dtab_ptr)  do {\
    dtab_ptr->len*=DTAB_GROWTH_FACTOR;\
    dtab_ptr->keys = realloc(dtab_ptr->keys, dtab_ptr->len * sizeof(dtab_hash_t));\
    dtab_ptr->values = realloc(dtab_ptr->values, dtab_ptr->len * dtab_ptr->bytesize);} while(0)
#define DTAB_FREE(dtab_ptr) do {free(dtab_ptr->keys) ;\
free(dtab_ptr->values);\
free(dtab_ptr); } while(0)

/* DTAB macros hash the input strings everytime.
Might be faster to put hash in variable and call functions directly? */
#define DTAB_ADD(dtab_ptr, value, name) dtab_add(dtab_ptr, &value, DTAB_HASH(name))
#define DTAB_ADDS(dtab_ptr, value, name) dtab_add(dtab_ptr, &value, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_ADDP(dtab_ptr, value, name) dtab_add(dtab_ptr, value, DTAB_HASH(name))
#define DTAB_ADDPS(dtab_ptr, value, name) dtab_add(dtab_ptr, value, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_GET(dtab_ptr, name) dtab_get(dtab_ptr, DTAB_HASH(name))
#define DTAB_GETS(dtab_ptr, name) dtab_get(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_DEL(dtab_ptr, name) dtab_del(dtab_ptr, DTAB_HASH(name))
#define DTAB_DELS(dtab_ptr, name) dtab_del(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_DEL_SCRAMBLE(dtab_ptr, name) dtab_del_scramble(dtab_ptr, DTAB_HASH(name))
#define DTAB_DEL_SCRAMBLES(dtab_ptr, name) dtab_del_scramble(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))

#endif /* DTAB */

/******************************** CONSTANTS *********************************/

enum NMATH_2DGRID_NEIGHBORS {
    NMATH_SQUARE_NEIGHBOURS = 4,
    NMATH_HEXAGON_NEIGHBOURS = 6,
};

enum NMATH_SIGHTMAP {
    NMATH_SIGHTMAP_BLOCKED = 0,
    NMATH_SIGHTMAP_VISIBLE = 1,
    NMATH_SIGHTMAP_OBSERVER = 2,
    NMATH_SIGHTMAP_WALL = 3,
    NMATH_SIGHTMAP_ENEMY = 4,
    NMATH_SIGHTMAP_FRIENDLY = 5,
};

enum NMATH_MODE_MOVETILE {
    NMATH_MOVETILE_EXCLUDE = 0,
    NMATH_MOVETILE_INCLUDE = 1,
};

enum NMATH_ATTACKMAP {
    NMATH_ATTACKMAP_BLOCKED = 0,
    NMATH_ATTACKMAP_MOVEABLEMIN = 1,
};

enum NMATH_ASSAILABLEMAP {
    NMATH_ASSAILABLE_BLOCKED = 0,
    NMATH_ASSAILABLE_MOVEABLEMIN = 1,
};

enum NMATH_PUSHPULLMAP {
    NMATH_PUSHPULLMAP_UNIT = 0,
    NMATH_PUSHPULLMAP_BLOCKED = -1,
    NMATH_PUSHPULLMAP_MINDIST = 1, // minimal moveable distance
};

enum NMATH_COSTMAP {
    NMATH_COSTMAP_BLOCKED = 0,
    NMATH_COSTMAP_MOVEABLEMIN = 1,
};

enum NMATH_MOVEMAP {
    NMATH_MOVEMAP_BLOCKED = 0,
    NMATH_MOVEMAP_MOVEABLEMIN = 1,
};

enum NMATH_BLOCKMAP {
    NMATH_BLOCKMAP_BLOCKED = 0,
    NMATH_BLOCKMAP_MIN = 1,
};

enum NMATH_GRADIENTMAP {
    NMATH_GRADIENTMAP_UNIT = 0,
    NMATH_GRADIENTMAP_BLOCKED = -1,
    NMATH_GRADIENTMAP_MINDIST = 1, // minimal moveable distance
};

enum NMATH_POINTS_MODE {
    NMATH_POINTS_MODE_MATRIX = 0,
    NMATH_POINTS_MODE_LIST = 1,
};

enum NMATH_MODE_PATHS {
    NMATH_PATH_STEP = 0, // i.e. relative path
    NMATH_PATH_POSITION = 1,  // i.e. absolute path
};

#define uint64_t_MAX UINT64_MAX
#define uint32_t_MAX UINT32_MAX
#define uint16_t_MAX UINT16_MAX
#define uint8_t_MAX UINT8_MAX
#define int64_t_MAX INT64_MAX
#define int32_t_MAX INT32_MAX
#define int16_t_MAX INT16_MAX
#define int8_t_MAX INT8_MAX

#ifndef TEMPLATE_TYPES_UINT
#define TEMPLATE_TYPES_UINT REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(uint64_t)
#endif  /* TEMPLATE_TYPES_UINT */

#ifndef TEMPLATE_TYPES_SINT
#define TEMPLATE_TYPES_SINT REGISTER_ENUM(int8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(int64_t)
#endif  /* TEMPLATE_TYPES_SINT */

#ifndef TEMPLATE_TYPES_INT
#define TEMPLATE_TYPES_INT  TEMPLATE_TYPES_SINT \
TEMPLATE_TYPES_UINT
#endif  /* TEMPLATE_TYPES_INT */

#ifndef TEMPLATE_TYPES_FLOAT
#define TEMPLATE_TYPES_FLOAT REGISTER_ENUM(float) \
REGISTER_ENUM(double)
#endif  /* TEMPLATE_TYPES_FLOAT */

#ifndef TEMPLATE_TYPES_BOOL
#define TEMPLATE_TYPES_BOOL REGISTER_ENUM(bool)
#endif  /* TEMPLATE_TYPES_BOOL */

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
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_point3D_##type {\
type x;\
type y;\
type z;\
} nmath_point3D_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hexpoint_##type {\
type x;\
type y;\
type z;\
} nmath_hexpoint_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_node_##type {\
type x;\
type y;\
type distance;\
} nmath_node_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_node3D_##type {\
type x;\
type y;\
type z;\
type distance;\
} nmath_node3D_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hexnode_##type {\
type x;\
type y;\
type z;\
type distance;\
} nmath_nodehex_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

/******************************** UTILITIES **********************************/

#define REGISTER_ENUM(type) extern type nmath_inbounds_##type(type pos, type boundmin, type boundmax);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

/************************* QUICK MATH *****************************/

#ifndef log2// because tcc SUCKS, does NOT DEFINE log2
#define log2(x) (log(x)/log(2.0f))
#endif

/************************* PERIODIC CYCLES *****************************/
// m:-1, p:1, z:0
#define q_cycle2_zp(i) (i % 2)
#define q_cycle2_pz(i) ((i + 1) % 2)
#define q_cycle2_mz(i) ((i % 2) - 1)
#define q_cycle2_zm(i) (((i + 1) % 2) - 1)
#define q_cycle2_mp(i) (q_cycle2_mz(i) + q_cycle2_zp(i))
#define q_cycle2_pm(i) (q_cycle2_pz(i) + q_cycle2_zm(i))

#define q_cycle3_mzp(i) ((i % 3) - 1)
#define q_cycle3_zpm(i) (((i + 1) % 3) - 1)
#define q_cycle3_pmz(i) (((i + 2) % 3) - 1)
#define q_cycle3_pzm(i) (1 - (i % 3))
#define q_cycle3_zmp(i) (1 - ((i + 1) % 3))
#define q_cycle3_mpz(i) (1 - ((i + 2) % 3))

#define q_cycle4_mzp2(i) ((i % 4) - 1)
#define q_cycle4_zp2m(i) (((i + 1) % 4) - 1)
#define q_cycle4_p2mz(i) (((i + 2) % 4) - 1)
#define q_cycle4_2mzp(i) (((i + 3) % 4) - 1)
#define q_cycle4_mzpz(i) (q_cycle2_pz(i) * q_cycle4_mzp2(i))
#define q_cycle4_pzmz(i) (q_cycle2_mz(i) * q_cycle4_mzp2(i))
#define q_cycle4_zmzp(i) (q_cycle2_zp(i) * q_cycle4_2mzp(i))
#define q_cycle4_zpzm(i) (q_cycle2_zp(i) * q_cycle4_zp2m(i))

#define q_cycle4_mmpp(i) (q_cycle4_mzpz(i) + q_cycle4_zmzp(i))
#define q_cycle4_pmmp(i) (q_cycle4_pzmz(i) + q_cycle4_zmzp(i))
#define q_cycle4_mppm(i) (q_cycle4_mzpz(i) + q_cycle4_zpzm(i))
#define q_cycle4_ppmm(i) (q_cycle4_pzmz(i) + q_cycle4_zpzm(i))

#define q_cycle6_mzpzzz(i) (q_cycle3_mzp(i) * q_cycle2_pz(i))
#define q_cycle6_zzzmzp(i) (q_cycle3_mzp(i) * q_cycle2_zp(i))
#define q_cycle6_zzmzpz(i) (q_cycle3_zpm(i) * q_cycle2_pz(i))
#define q_cycle6_zpzzzm(i) (q_cycle3_zpm(i) * q_cycle2_zp(i))
#define q_cycle6_pzzzmz(i) (q_cycle3_pmz(i) * q_cycle2_pz(i))
#define q_cycle6_zmzpzz(i) (q_cycle3_pmz(i) * q_cycle2_zp(i))

#define q_cycle6_mzzzpz(i) (q_cycle3_mpz(i) * q_cycle2_pz(i))
#define q_cycle6_zpzmzz(i) (q_cycle3_mpz(i) * q_cycle2_zp(i))
#define q_cycle6_pzmzzz(i) (q_cycle3_pzm(i) * q_cycle2_pz(i))
#define q_cycle6_zzzpzm(i) (q_cycle3_pzm(i) * q_cycle2_zp(i))
#define q_cycle6_zzpzmz(i) (q_cycle3_zmp(i) * q_cycle2_pz(i))
#define q_cycle6_zmzzzp(i) (q_cycle3_zmp(i) * q_cycle2_zp(i))

#define q_cycle6_mppmzz(i) (q_cycle6_mzpzzz(i) + q_cycle6_zpzmzz(i))
#define q_cycle6_zzmppm(i) (q_cycle6_zzzpzm(i) + q_cycle6_zzmzpz(i))
#define q_cycle6_pmzzmp(i) (q_cycle6_zmzzzp(i) + q_cycle6_pzzzmz(i))

#define REGISTER_ENUM(type) extern type q_sequence_geometric_##type(type start, type destination, type geo_factor);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type q_sequence_pingpong_##type(type current, type upper, type lower);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern float q_sqrt_##type(type in_int);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define  carmack_sqrt_int8_t q_sqrt_int8_t
#define  carmack_sqrt_uint8_t q_sqrt_uint8_t
#define  carmack_sqrt_int16_t q_sqrt_int16_t
#define  carmack_sqrt_uint16_t q_sqrt_uint16_t
#define  carmack_sqrt_int32_t q_sqrt_int32_t
#define  carmack_sqrt_uint32_t q_sqrt_uint32_t
#define  carmack_sqrt_int64_t q_sqrt_int64_t
#define  carmack_sqrt_uint64_t q_sqrt_uint64_t
#define  carmack_sqrt_float q_sqrt_float
#define  carmack_sqrt_double q_sqrt_double

/********************************* LINALG ************************************/

// linalg uses unraveled arrays as n-dimensional matrices
// col->x, row->y, depth->z
#define linalg_index_arr2D(row, col, col_len) (row * col_len + col)
#define linalg_index_arr3D(row, col, depth, row_len, col_len) (row * col_len * depth_len + col * depth_len + depth)

#define REGISTER_ENUM(type) extern type linalg_determinant_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_trace_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_isIn_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_1D_##type(type * list_1D, size_t list_len, type x);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_2D_##type(type * list_1D, size_t list_len, type x, type y);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_3D_##type(type * list_1D, size_t list_len, type x, type y, type z);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * linalg_where_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern void linalg_matrix_print_##type(type * array, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance);
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_any_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_all_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_minus_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mult_scalar_##type(type * matrix1, type mult, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mult_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
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

/******************************* PATHFINDING ***********************************/

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Block_##type(type * costmap_pushpull, size_t row_len, size_t col_len, struct nmath_point_##type start);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Pushable_##type(type * assailablemap, size_t row_len, size_t col_len, type range[2], struct nmath_point_##type target);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type pathfinding_Direction_Pullable_##type(type * assailablemap, size_t row_len, size_t col_len, type range[2], struct nmath_point_##type target);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_PushPullable_##type(struct nmath_sq_neighbors_##type direction_block, struct nmath_sq_neighbors_##type pushpullable, size_t row_len, size_t col_len, struct nmath_point_##type start, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Movable_##type(type * costmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type move, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_##type(type * blockmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type sight, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Attack_##type(type * move_matrix, size_t row_len, size_t col_len, type move, type range[2], uint8_t mode_output, uint8_t mode_movetile);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Movable_Hex_##type(type * costmap, size_t row_len, size_t depth_len, struct nmath_hexpoint_##type  start, type move, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_Hex_##type(type * blockmap, size_t depth_len, size_t col_len, struct nmath_hexpoint_##type  start, type sight, uint8_t mode);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Assailable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, type range[2], uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Pushable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Pullable_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_unitGradient_##type(type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Map_Path_##type(type * move_matrix, size_t row_len, size_t col_len, struct nmath_point_##type start, struct nmath_point_##type end, uint8_t mode_path);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Path_step2position_##type(type * step_list, size_t list_len, struct nmath_point_##type start);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * pathfinding_Path_position2step_##type(type * position_list, size_t list_len);
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#endif /* NOURSMATH_H */