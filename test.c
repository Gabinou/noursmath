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
    }\
} while (0)

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

void test_uint8_t() {
    uint8_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(uint8_t));
    uint8_t * tempvec1 = calloc(3, sizeof(uint8_t));
    uint8_t * tempvec2 = calloc(3, sizeof(uint8_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    uint8_t * out = NULL;
    out = linalg_matrix_plus_uint8_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_uint8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint8_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint8_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_uint8_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint8_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint8_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint8_t * tempvec3 = linalg_crossProduct_uint8_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_uint8_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint8_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint8_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint8_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint8_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint8_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    uint8_t * matrixed2d = linalg_list2matrix_uint8_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_uint8_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    uint8_t * listeded2d = linalg_matrix2list_uint8_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint8_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void test_int8_t() {
    int8_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(int8_t));
    int8_t * tempvec1 = calloc(3, sizeof(int8_t));
    int8_t * tempvec2 = calloc(3, sizeof(int8_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    int8_t * out = NULL;
    out = linalg_matrix_plus_int8_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_int8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int8_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    int8_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_int8_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int8_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int8_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int8_t * tempvec3 = linalg_crossProduct_int8_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_int8_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int8_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int8_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int8_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int8_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int8_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    int8_t * matrixed2d = linalg_list2matrix_int8_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_int8_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    int8_t * listeded2d = linalg_matrix2list_int8_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int8_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void test_uint16_t() {
    uint16_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(uint16_t));
    uint16_t * tempvec1 = calloc(3, sizeof(uint16_t));
    uint16_t * tempvec2 = calloc(3, sizeof(uint16_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    uint16_t * out = NULL;
    out = linalg_matrix_plus_uint16_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_uint16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint16_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint16_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_uint16_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint16_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint16_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint16_t * tempvec3 = linalg_crossProduct_uint16_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_uint16_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint16_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint16_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint16_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    uint16_t * matrixed2d = linalg_list2matrix_uint16_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_uint16_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    uint16_t * listeded2d = linalg_matrix2list_uint16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint16_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}
void test_int16_t() {
    int16_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(int16_t));
    int16_t * tempvec1 = calloc(3, sizeof(int16_t));
    int16_t * tempvec2 = calloc(3, sizeof(int16_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    int16_t * out = NULL;
    out = linalg_matrix_plus_int16_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int16_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    int16_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_int16_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int16_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int16_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int16_t * tempvec3 = linalg_crossProduct_int16_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_int16_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int16_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int16_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int16_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    int16_t * matrixed2d = linalg_list2matrix_int16_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_int16_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    int16_t * listeded2d = linalg_matrix2list_int16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int16_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}
void test_uint32_t() {
    uint32_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(uint32_t));
    uint32_t * tempvec1 = calloc(3, sizeof(uint32_t));
    uint32_t * tempvec2 = calloc(3, sizeof(uint32_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    uint32_t * out = NULL;
    out = linalg_matrix_plus_uint32_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_uint32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint32_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint32_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_uint32_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint32_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint32_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint32_t * tempvec3 = linalg_crossProduct_uint32_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_uint32_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint32_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint32_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint32_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint32_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint32_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    uint32_t * matrixed2d = linalg_list2matrix_uint32_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_uint32_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    uint32_t * listeded2d = linalg_matrix2list_uint32_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint32_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}
void test_int32_t() {
    int32_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(int32_t));
    int32_t * tempvec1 = calloc(3, sizeof(int32_t));
    int32_t * tempvec2 = calloc(3, sizeof(int32_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    int32_t * out = NULL;
    out = linalg_matrix_plus_int32_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_int32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int32_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    int32_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_int32_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int32_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int32_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int32_t * tempvec3 = linalg_crossProduct_int32_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_int32_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int32_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int32_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int32_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int32_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int32_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    int32_t * matrixed2d = linalg_list2matrix_int32_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_int32_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    int32_t * listeded2d = linalg_matrix2list_int32_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int32_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}
void test_uint64_t() {
    uint64_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(uint64_t));
    uint64_t * tempvec1 = calloc(3, sizeof(uint64_t));
    uint64_t * tempvec2 = calloc(3, sizeof(uint64_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    uint64_t * out = NULL;
    out = linalg_matrix_plus_uint64_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_uint64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint64_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint64_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_uint64_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint64_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint64_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint64_t * tempvec3 = linalg_crossProduct_uint64_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_uint64_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint64_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_uint64_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint64_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_uint64_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    uint64_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    uint64_t * matrixed2d = linalg_list2matrix_uint64_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_uint64_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    uint64_t * listeded2d = linalg_matrix2list_uint64_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_uint64_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void test_int64_t() {
    int64_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(int64_t));
    int64_t * tempvec1 = calloc(3, sizeof(int64_t));
    int64_t * tempvec2 = calloc(3, sizeof(int64_t));
    tempvec1[0] = 1; 
    tempvec1[1] = 2; 
    tempvec1[2] = 3;     
    tempvec2[0] = 4; 
    tempvec2[1] = 5; 
    tempvec2[2] = 6; 
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    int64_t * out = NULL;
    out = linalg_matrix_plus_int64_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);
    lok(linalg_equal_int64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int64_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    int64_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    size_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }
    lok(linalg_trace_int64_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int64_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int64_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int64_t * tempvec3 = linalg_crossProduct_int64_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out = linalg_matrix_and_int64_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int64_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_and_int64_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int64_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    out = linalg_matrix_or_int64_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(out, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    free(out);
    int64_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    int64_t * matrixed2d = linalg_list2matrix_int64_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    lok(linalg_equal_int64_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    int64_t * listeded2d = linalg_matrix2list_int64_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    lok(linalg_equal_int64_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void test_float() {


}

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