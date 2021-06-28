#include "linalg.h"


#define REGISTER_ENUM(type) type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len) {\
    type out = 0;\
    for (size_t i = 0; i < arr_len; i++) {\
        out += arr1[i] * arr2[i];\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_1D_##type(type * list_2D, size_t list_len, type x) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if (x == list_2D[i * ONE_D + 0]) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_2D_##type(type * list_2D, size_t list_len, type x, type y) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_2D[i * TWO_D + 0]) && (y == list_2D[i * TWO_D + 1])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_3D_##type(type * list_3D, size_t list_len, type x, type y, type z) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_3D[i * THREE_D + 0]) && (y == list_3D[i * THREE_D + 1]) && (z == list_3D[i * THREE_D + 2])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_array_isIn_##type(type * array, type to_find, size_t arr_len) {\
    bool found = false;\
    for (size_t i = 0; i < arr_len; i++) {\
        if (array[i] == to_find) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) size_t * linalg_array_where_##type(type * array, type to_find, size_t arr_len) {\
    size_t * found_list = DARR_INIT(found_list, size_t, arr_len);\
    DARR_LEN(found_list) = arr_len;\
    DARR_NUM(found_list) = 0;\
    for (size_t i = 0; i < arr_len; i++) {\
        if (array[i] == to_find) {\
            found_list[DARR_NUM(found_list)++]= i;\
            break;\
        }\
    }\
    DARR_LEN(found_list) = DARR_NUM(found_list);\
    found_list = DARR_REALLOC(found_list, DARR_NUM(found_list));\
    return (found_list);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

// size_t * found_list = (size_t* )malloc(sizeof(size_t)*(row_len*col_len + 1)) + 1;
#define REGISTER_ENUM(type) size_t * linalg_matrix_where_##type(type * matrix, type to_find, size_t row_len, size_t col_len) {\
    size_t * found_list = DARR_INIT(found_list, size_t, row_len*col_len);\
    DARR_LEN(found_list) = row_len*col_len;\
    DARR_NUM(found_list) = 0;\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            if (matrix[row * col_len + col] == to_find) {\
                found_list[DARR_NUM(found_list)++] = col;\
                found_list[DARR_NUM(found_list)++] = row;\
            }\
        }\
    }\
    DARR_LEN(found_list) = DARR_NUM(found_list);\
    found_list = DARR_REALLOC(found_list, DARR_NUM(found_list));\
    return (found_list);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM
#define REGISTER_ENUM(type) bool linalg_matrix_isIn_##type(type * matrix, type to_find, size_t row_len, size_t col_len) {\
    bool found = false;\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            if (matrix[row * col_len + col] == to_find) {\
                found = false;\
                goto OUT;\
            }\
        }\
    }\
    OUT:;\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t elem = 0; elem < list_len; elem++) {\
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) void matrix_print_##type(type * array, size_t row_len, size_t col_len) {\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            printf("%02d ", array[row * col_len + col]);\
        }\
        printf("\n");\
    }\
}

#define REGISTER_ENUM(type) bool linalg_matrix_equal_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len) {\
    bool equal = true;\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            if (matrix1[row * col_len + col] != matrix2[row * col_len + col]) {\
                equal = false;\
                goto OUT;\
            }\
        }\
    }\
OUT:;\
return (equal);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix_and_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            out[row * col_len + col] = (matrix1[row * col_len + col] && matrix2[row * col_len + col]);\
        }\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix_or_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            out[row * col_len + col] = matrix1[row * col_len + col] || matrix2[row * col_len + col];\
        }\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix_plus_##type(type * matrix1, type * matrix2, size_t row_len, size_t col_len, int8_t sign) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            out[row * col_len + col] = matrix1[row * col_len + col] + sign * matrix2[row * col_len + col];\
        }\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) type * linalg_matrix_mask_##type(type * matrix, type * mask, size_t row_len, size_t col_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            out[row * col_len + col] = matrix[row * col_len + col] * (mask[row * col_len + col] > 0);\
        }\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len) {\
    type * list = DARR_INIT(list, type, row_len*col_len*2);\
    DARR_LEN(list) = row_len*col_len*2;\
    DARR_NUM(list) = 0;\
    for (size_t col = 0; col < col_len; col++) {\
        for (size_t row = 0; row < row_len; row++) {\
            if (matrix[row * col_len + col] > 0) {\
                list[DARR_NUM(list)++] = col;\
                list[DARR_NUM(list)++] = row;\
            }\
        }\
    }\
    type * out = DARR_INIT(out, type, DARR_NUM(list));\
    DARR_LEN(out) = DARR_NUM(list);\
    DARR_NUM(out) = DARR_NUM(list);\
    out = memcpy(out, list, DARR_NUM(list)*sizeof(type));\
    DARR_FREE(list);\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM



