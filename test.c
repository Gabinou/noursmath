#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "linalg.h"

/* MINCTEST - Minimal C Test Library - 0.2.0
*  ---------> MODIFIED FOR linalg <----------
* Copyright (c) 2014-2017 Lewis Van Winkle
*
* http://CodePlea.com
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgement in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __MINCTEST_H__
#define __MINCTEST_H__
/*****************************TEST GLOBALS*****************************/
FILE * globalf;
/* NB all tests should be in one file. */
static int ltests = 0;
static int lfails = 0;

/* Display the test results. */
#define lresults() do {\
    if (lfails == 0) {\
        dupprintf(globalf,"ALL TESTS PASSED (%d/%d)\n", ltests, ltests);\
    } else {\
        dupprintf(globalf,"SOME TESTS FAILED (%d/%d)\n", ltests-lfails, ltests);\
    }\
} while (0)

/* Run a test. Name can be any string to print out, test is the function name to call. */
#define lrun(name, test) do {\
    const int ts = ltests;\
    const int fs = lfails;\
    const clock_t start = clock();\
    dupprintf(globalf,"\t%-14s", name);\
    test();\
    dupprintf(globalf,"pass:%2d   fail:%2d   %4dms\n",\
            (ltests-ts)-(lfails-fs), lfails-fs,\
            (int)((clock() - start) * 1000 / CLOCKS_PER_SEC));\
} while (0)

/* Assert a true statement. */
#define lok(test) do {\
    ++ltests;\
    if (!(test)) {\
        ++lfails;\
        dupprintf(globalf,"%s:%d error \n", __FILE__, __LINE__);\
    }} while (0)

#endif /*__MINCTEST_H__*/

void dupprintf(FILE * f, char const * fmt, ...) { // duplicate printf
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

/*****************************TEST CONSTANTS***************************/
#define ITERATIONS 10000
#define ARRAY_LEN 100
#define STRINGIFY(x) #x
size_t fps_iterations = 10;

/*******************************ACTUAL TESTS***************************/
void test_log2() {
    lok(log2(0.0) == -INFINITY);
    lok(log2(0.0) == -INFINITY);
    lok(log2(0) == -INFINITY);
    lok(log2(0) == -INFINITY);
    lok(log2(1.0) == 0.0);
    lok(log2(1.0) == 0);
    lok(log2(2.0) == 1.0);
    lok(log2(2.0) == 1);
}

enum {
    LINALG_ROW_LEN = 21,
    LINALG_COL_LEN = 25,
    LINALG_SQUARE = 4,
};
#define REGISTER_ENUM(type) void test_##type() {\
    type * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(type));\
    type * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(type));\
    type * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(type));\
    type * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(type));\
    type * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(type));\
    type * tempvec1 = calloc(3, sizeof(type));\
    type * tempvec2 = calloc(3, sizeof(type));\
    tempvec1[0] = 1; \
    tempvec1[1] = 2; \
    tempvec1[2] = 3;     \
    tempvec2[0] = 4; \
    tempvec2[1] = 5; \
    tempvec2[2] = 6; \
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {\
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {\
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;\
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;\
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;\
        }\
    }\
    type * out = NULL;\
    out = linalg_matrix_plus_##type(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);\
    lok(linalg_matrix_equal_##type(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    type tempssq[LINALG_SQUARE * LINALG_SQUARE] = {\
         1,  2,  3,  4,\
         5,  6,  7,  8,\
         9, 10, 11, 12,\
        13, 14, 15, 16\
    };\
    type temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {\
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0\
    };\
    size_t dot_prod = 0;\
    for (size_t row = 0; row < LINALG_SQUARE; row++) {\
        for (size_t col = 0; col < LINALG_SQUARE; col++) {\
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];\
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];\
        }\
    }\
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {\
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {\
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];\
        }\
    }\
    lok(linalg_trace_##type(tempsq, LINALG_SQUARE) == 34);\
    lok(linalg_trace_##type(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));\
    lok(linalg_dotProduct_##type(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);\
    type * tempvec3 = linalg_crossProduct_##type(tempvec1, tempvec2);\
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);\
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);\
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);\
    out = linalg_matrix_and_##type(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    out = linalg_matrix_and_##type(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    out = linalg_matrix_and_##type(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    out = linalg_matrix_or_##type(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    out = linalg_matrix_or_##type(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));\
    free(out);\
    type templist1[2 * 16] = {\
        0, 0,\
        0, 1,\
        0, 2,\
        0, 3,\
        0, 4,\
        0, 5,\
        1, 0,\
        1, 1,\
        1, 2,\
        1, 3,\
        2, 0,\
        2, 1,\
        2, 2,\
        3, 0,\
        3, 1,\
        4, 0\
    };\
    type * matrixed2d = linalg_list2matrix_##type(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);\
    lok(linalg_matrix_equal_##type(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));\
    type * listeded2d = linalg_matrix2list_##type(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);\
    lok(linalg_matrix_equal_##type(listeded2d, listeded2d, 2, 16));\
    lok(DARR_NUM(listeded2d) == 16 * 2);\
    free(temp2D1);\
    free(temp2D2);\
    free(temp2D3);\
    free(temp2D4);\
    free(tempsq);\
    free(tempvec1);\
    free(tempvec2);\
    free(tempvec3);\
    free(matrixed2d);\
    DARR_FREE(listeded2d);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

int main() {
    globalf = fopen("linalg_test_results.txt", "w+");
    dupprintf(globalf, "\nHello, World! I am testing linalg.\n");
    lrun("log2", test_log2);
    #define REGISTER_ENUM(type) lrun(STRINGIFY(test_##type), test_##type);
    LINALG_TEMPLATE_TYPES_INT
    #undef REGISTER_ENUM
    lresults();

    dupprintf(globalf, "linalg Test End \n \n");
    fclose(globalf);
    return (0);
}