#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "noursmath.h"

/********************** 0.1 MICROSECOND RESOLUTION CLOCK **********************/
//  Modified from: https://gist.github.com/ForeverZer0/0a4f80fc02b96e19380ebb7a3debbee5
#if defined(__linux)
#  define MICROSECOND_CLOCK
#  define HAVE_POSIX_TIMER
#  include <time.h>
#  ifdef CLOCK_MONOTONIC
#     define CLOCKID CLOCK_MONOTONIC
#  else
#     define CLOCKID CLOCK_REALTIME
#  endif
#elif defined(__APPLE__)
#  define MICROSECOND_CLOCK
#  define HAVE_MACH_TIMER
#  include <mach/mach_time.h>
#elif defined(_WIN32)
#  define MICROSECOND_CLOCK
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

uint64_t get_ns() {
    static uint64_t is_init = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (0 == is_init) {
        mach_timebase_info(&info);
        is_init = 1;
    }
    uint64_t now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
#elif defined(__linux)
    static struct timespec linux_rate;
    if (0 == is_init) {
        clock_getres(CLOCKID, &linux_rate);
        is_init = 1;
    }
    uint64_t now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
#elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (0 == is_init) {
        QueryPerformanceFrequency(&win_frequency);
        is_init = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t)((1e9 * now.QuadPart) / win_frequency.QuadPart);
#endif
}
#ifdef MICROSECOND_CLOCK
double get_us() {
    return (get_ns() / 1e3);
}
#else
#  define FAILSAFE_CLOCK
#  define get_us() (((double)clock())/CLOCKS_PER_SEC*1e6) // [us]
#  define get_ns() (((double)clock())/CLOCKS_PER_SEC*1e9) // [ns]
#endif


/* MINCTEST - Minimal C Test Library - 0.2.0
*  ---------> MODIFIED FOR NOURSMATH <----------
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
    dupprintf(globalf,"\t\tpass:%2d   fail:%2d   %4dms\n",\
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

void dupprintf(FILE * f, char const * fmt, ...) { // duplicate printf
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}

#endif /*__MINCTEST_H__*/

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

void linalg_bool() {

}

void linalg_uint8_t() {
    uint8_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
    uint8_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint8_t));
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
    uint8_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_uint8_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_uint8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint8_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint8_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_uint8_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_uint8_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    uint8_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint8_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint8_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_uint8_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_uint8_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint8_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint8_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint8_t * tempvec3 = linalg_crossProduct_uint8_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_uint8_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint8_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint8_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint8_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint8_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint8_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint8_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_uint8_t(templist1, 2 * 16));
    lok(!linalg_any_uint8_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_uint8_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_uint8_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_uint8_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_uint8_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_uint8_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_uint8_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_uint8_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_uint8_t(templist1, 8, 2 * 16));
    out = linalg_where_uint8_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    uint8_t * matrixed2d = linalg_list2matrix_uint8_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_uint8_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint8_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    uint8_t * listeded2d = linalg_matrix2list_uint8_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_uint8_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_uint8_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_int8_t() {
    int8_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
    int8_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int8_t));
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
    int8_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_int8_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_int8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int8_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    int8_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_int8_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_int8_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    int8_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int8_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int8_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_int8_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_int8_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int8_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int8_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int8_t * tempvec3 = linalg_crossProduct_int8_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_int8_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int8_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int8_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int8_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int8_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int8_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int8_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_int8_t(templist1, 2 * 16));
    lok(!linalg_any_int8_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_int8_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_int8_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_int8_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_int8_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_int8_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_int8_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_int8_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_int8_t(templist1, 8, 2 * 16));
    out = linalg_where_int8_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    int8_t * matrixed2d = linalg_list2matrix_int8_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_int8_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int8_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    int8_t * listeded2d = linalg_matrix2list_int8_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_int8_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_int8_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_uint16_t() {
    uint16_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
    uint16_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint16_t));
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
    uint16_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_uint16_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_uint16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint16_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint16_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_uint16_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_uint16_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    uint16_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint16_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint16_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_uint16_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_uint16_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint16_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint16_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint16_t * tempvec3 = linalg_crossProduct_uint16_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_uint16_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint16_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint16_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint16_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint16_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint16_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint16_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_uint16_t(templist1, 2 * 16));
    lok(!linalg_any_uint16_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_uint16_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_uint16_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_uint16_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_uint16_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_uint16_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_uint16_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_uint16_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_uint16_t(templist1, 8, 2 * 16));
    out = linalg_where_uint16_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    uint16_t * matrixed2d = linalg_list2matrix_uint16_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_uint16_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint16_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    uint16_t * listeded2d = linalg_matrix2list_uint16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_uint16_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_uint16_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_int16_t() {
    int16_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
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
    int16_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_int16_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_int16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int16_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    int16_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_int16_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_int16_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    int16_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int16_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int16_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_int16_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_int16_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int16_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int16_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int16_t * tempvec3 = linalg_crossProduct_int16_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_int16_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int16_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int16_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int16_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int16_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_int16_t(templist1, 2 * 16));
    lok(!linalg_any_int16_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_int16_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_int16_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_int16_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_int16_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_int16_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_int16_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_int16_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_int16_t(templist1, 8, 2 * 16));
    out = linalg_where_int16_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    int16_t * matrixed2d = linalg_list2matrix_int16_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_int16_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int16_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    int16_t * listeded2d = linalg_matrix2list_int16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_int16_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_int16_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_uint32_t() {
    uint32_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
    uint32_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint32_t));
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
    uint32_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_uint32_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_uint32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint32_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint32_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_uint32_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_uint32_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    uint32_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint32_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint32_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_uint32_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_uint32_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint32_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint32_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint32_t * tempvec3 = linalg_crossProduct_uint32_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_uint32_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint32_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint32_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint32_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint32_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint32_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint32_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_uint32_t(templist1, 2 * 16));
    lok(!linalg_any_uint32_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_uint32_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_uint32_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_uint32_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_uint32_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_uint32_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_uint32_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_uint32_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_uint32_t(templist1, 8, 2 * 16));
    out = linalg_where_uint32_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    uint32_t * matrixed2d = linalg_list2matrix_uint32_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_uint32_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint32_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    uint32_t * listeded2d = linalg_matrix2list_uint32_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_uint32_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_uint32_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_int32_t() {
    int32_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
    int32_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int32_t));
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
    int32_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_int32_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_int32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int32_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    int32_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_int32_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_int32_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    int32_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int32_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int32_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_int32_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_int32_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int32_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int32_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int32_t * tempvec3 = linalg_crossProduct_int32_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_int32_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int32_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int32_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int32_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int32_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int32_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int32_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_int32_t(templist1, 2 * 16));
    lok(!linalg_any_int32_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_int32_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_int32_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_int32_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_int32_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_int32_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_int32_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_int32_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_int32_t(templist1, 8, 2 * 16));
    out = linalg_where_int32_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    int32_t * matrixed2d = linalg_list2matrix_int32_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_int32_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int32_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    int32_t * listeded2d = linalg_matrix2list_int32_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_int32_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_int32_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_uint64_t() {
    uint64_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
    uint64_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(uint64_t));
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
    uint64_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_uint64_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_uint64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint64_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    uint64_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_uint64_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_uint64_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    uint64_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint64_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    uint64_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_uint64_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_uint64_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_uint64_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_uint64_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    uint64_t * tempvec3 = linalg_crossProduct_uint64_t(tempvec1, tempvec2);
    // lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    // lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    // lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_uint64_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint64_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_uint64_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint64_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_uint64_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_uint64_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    uint64_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_uint64_t(templist1, 2 * 16));
    lok(!linalg_any_uint64_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));

    lok(linalg_list_isIn_2D_uint64_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_uint64_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_uint64_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_uint64_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_uint64_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_uint64_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_uint64_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_uint64_t(templist1, 8, 2 * 16));
    out = linalg_where_uint64_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    uint64_t * matrixed2d = linalg_list2matrix_uint64_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_uint64_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_uint64_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    uint64_t * listeded2d = linalg_matrix2list_uint64_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_uint64_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_uint64_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_int64_t() {
    int64_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
    int64_t * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int64_t));
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
    int64_t * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_int64_t(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_int64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int64_t tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    int64_t tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_int64_t(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_int64_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
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

    int64_t temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int64_t temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    int64_t dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_int64_t(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    lok(linalg_trace_int64_t(tempsq, LINALG_SQUARE) == 34);
    lok(linalg_trace_int64_t(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_int64_t(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    int64_t * tempvec3 = linalg_crossProduct_int64_t(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_int64_t(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int64_t(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_int64_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int64_t(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_int64_t(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_int64_t(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    int64_t templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_int64_t(templist1, 2 * 16));
    lok(!linalg_any_int64_t(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
    lok(linalg_list_isIn_2D_int64_t(templist1, 2 * 16, 0, 1));
    lok(!linalg_list_isIn_2D_int64_t(templist1, 2 * 16, 4, 1));
    lok(linalg_list_isIn_1D_int64_t(templist1, 2 * 16, 4));
    lok(!linalg_list_isIn_1D_int64_t(templist1, 2 * 16, 7));
    lok(linalg_list_isIn_3D_int64_t(templist1, 2 * 16, 1, 0, 2));
    lok(!linalg_list_isIn_3D_int64_t(templist1, 2 * 16, 4, 4, 4));
    lok(linalg_isIn_int64_t(templist1, 4, 2 * 16));
    lok(!linalg_isIn_int64_t(templist1, 8, 2 * 16));
    out = linalg_where_int64_t(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    int64_t * matrixed2d = linalg_list2matrix_int64_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_int64_t(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_int64_t(out, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out);
    int64_t * listeded2d = linalg_matrix2list_int64_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_int64_t(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_int64_t(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void pathfinding_Map_Visiblebool() {
    bool * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * temp2D5 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * temp2D6 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(bool));
    bool * tempsq = calloc(LINALG_SQUARE * LINALG_SQUARE, sizeof(bool));
    bool * tempvec1 = calloc(3, sizeof(bool));
    bool * tempvec2 = calloc(3, sizeof(bool));
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
    bool * out1 = NULL, * out2 = NULL, *out = NULL;
    out1 = linalg_plus_bool(temp2D3, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN, -1);
    out2 = linalg_equal_bool(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    // lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    bool tempssq[LINALG_SQUARE * LINALG_SQUARE] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };
    bool tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    lok(linalg_all_bool(tempssq, LINALG_SQUARE * LINALG_SQUARE));
    lok(!linalg_all_bool(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
    bool temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
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

    bool temp2D22[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    bool temp2D33[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 3, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

    bool dot_prod = 0;
    for (size_t row = 0; row < LINALG_SQUARE; row++) {
        for (size_t col = 0; col < LINALG_SQUARE; col++) {
            tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] = tempssq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
            dot_prod += tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)] * tempsq[linalg_index_arr2D(row, col, LINALG_SQUARE)];
        }
    }
    for (size_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (size_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
            temp2D5[(row * LINALG_COL_LEN + col)] = temp2D22[(row * LINALG_COL_LEN + col)];
            temp2D6[(row * LINALG_COL_LEN + col)] = temp2D33[(row * LINALG_COL_LEN + col)];
        }
    }

    out1 = linalg_mask_bool(temp2D5, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D6, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);

    // lok(linalg_trace_bool(tempsq, LINALG_SQUARE) == 34);
    // lok(linalg_trace_bool(tempsq, LINALG_SQUARE) == (tempssq[0] + tempssq[5] + tempssq[10] + tempssq[15]));
    lok(linalg_dotProduct_bool(tempsq, tempsq, LINALG_SQUARE * LINALG_SQUARE) == dot_prod);
    bool * tempvec3 = linalg_crossProduct_bool(tempvec1, tempvec2);
    lok(tempvec3[0] == tempvec1[1] * tempvec2[2] - tempvec1[2] * tempvec2[1]);
    lok(tempvec3[1] == tempvec1[2] * tempvec2[0] - tempvec1[0] * tempvec2[2]);
    lok(tempvec3[2] == tempvec1[0] * tempvec2[1] - tempvec1[1] * tempvec2[0]);
    out1 = linalg_and_bool(temp2D1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_bool(temp2D1, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_and_bool(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_bool(temp2D2, temp2D2, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D1, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    out1 = linalg_or_bool(temp2D11, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    out2 = linalg_equal_bool(out1, temp2D4, LINALG_ROW_LEN * LINALG_COL_LEN);
    lok(linalg_all_bool(out2, LINALG_ROW_LEN * LINALG_COL_LEN));
    free(out1);
    free(out2);
    bool templist1[2 * 16] = {
        0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 1, 0, 1, 1,
        1, 2, 1, 3, 2, 0, 2, 1, 2, 2, 3, 0, 3, 1, 4, 0
    };
    lok(linalg_any_bool(templist1, 2 * 16));
    lok(!linalg_any_bool(tempssq2, LINALG_SQUARE * LINALG_SQUARE));
    lok(linalg_list_isIn_2D_bool(templist1, 2 * 16, 0, 1));
    // lok(!linalg_list_isIn_2D_bool(templist1, 2*16, 4, 1));
    lok(linalg_list_isIn_1D_bool(templist1, 2 * 16, 4));
    // lok(!linalg_list_isIn_1D_bool(templist1, 2*16, 7));
    lok(linalg_list_isIn_3D_bool(templist1, 2 * 16, 1, 0, 2));
    // lok(!linalg_list_isIn_3D_bool(templist1, 2*16, 4, 4, 4));
    lok(linalg_isIn_bool(templist1, 4, 2 * 16));
    // lok(!linalg_isIn_bool(templist1, 8, 2*16));
    out = linalg_where_bool(templist1, 4, 2 * 16);
    lok(out[0] = 10);
    lok(out[1] = 30);
    DARR_FREE(out);
    bool * matrixed2d = linalg_list2matrix_bool(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    out = linalg_equal_bool(matrixed2d, temp2D11, LINALG_ROW_LEN * LINALG_COL_LEN);
    // lok(linalg_all_bool(out, LINALG_ROW_LEN* LINALG_COL_LEN));
    free(out);
    bool * listeded2d = linalg_matrix2list_bool(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    out = linalg_equal_bool(listeded2d, listeded2d, 16 * 2);
    lok(linalg_all_bool(out, 16 * 2));
    free(out);
    lok(DARR_NUM(listeded2d) == 16 * 2);
    free(temp2D1);
    free(temp2D2);
    free(temp2D3);
    free(temp2D4);
    free(temp2D5);
    free(temp2D6);
    free(tempsq);
    free(tempvec1);
    free(tempvec2);
    free(tempvec3);
    free(matrixed2d);
    DARR_FREE(listeded2d);
}

void linalg_float() {
    dupprintf(globalf, "\nlinalg_float\n");
    float tempssq1[LINALG_SQUARE * LINALG_SQUARE] = {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    float tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        1.1f,  2.01f,  3.001f,  4.0001f,
        5.1f,  6.01f,  7.001f,  8.0001f,
        9.1f, 10.01f, 11.001f, 12.0001f,
        13.1f, 14.01f, 15.001f, 16.0001f
    };
    bool tempssq3[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 1, 1, 1,
        0, 1, 1, 1,
        0, 1, 1, 1,
        0, 1, 1, 1
    };
    bool tempssq4[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1
    };
    bool tempssq5[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1
    };
    bool tempssq6[LINALG_SQUARE * LINALG_SQUARE] = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    };
    bool tempssq7[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    bool * out1 = NULL, * out2 = NULL;

    // matrix_print_bool(out1, LINALG_SQUARE, LINALG_SQUARE);
    // matrix_print_bool(out2, LINALG_SQUARE, LINALG_SQUARE);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 1.0f);
    out2 = linalg_equal_bool(out1, tempssq6, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.2f);
    out2 = linalg_equal_bool(out1, tempssq6, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.02f);
    out2 = linalg_equal_bool(out1, tempssq3, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.002f);
    out2 = linalg_equal_bool(out1, tempssq4, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.0002f);
    out2 = linalg_equal_bool(out1, tempssq5, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_float(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.00001f);
    out2 = linalg_equal_bool(out1, tempssq7, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
}

void linalg_double() {
    dupprintf(globalf, "\nlinalg_double\n");
    double tempssq1[LINALG_SQUARE * LINALG_SQUARE] = {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    double tempssq2[LINALG_SQUARE * LINALG_SQUARE] = {
        1.1f,  2.01f,  3.001f,  4.0001f,
        5.1f,  6.01f,  7.001f,  8.0001f,
        9.1f, 10.01f, 11.001f, 12.0001f,
        13.1f, 14.01f, 15.001f, 16.0001f
    };
    bool tempssq3[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 1, 1, 1,
        0, 1, 1, 1,
        0, 1, 1, 1,
        0, 1, 1, 1
    };
    bool tempssq4[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1
    };
    bool tempssq5[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 1
    };
    bool tempssq6[LINALG_SQUARE * LINALG_SQUARE] = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    };
    bool tempssq7[LINALG_SQUARE * LINALG_SQUARE] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    bool * out1 = NULL, * out2 = NULL;

    out1 = linalg_equal_double(tempssq1, tempssq2, (size_t)(LINALG_SQUARE * LINALG_SQUARE), 1.0f);
    out2 = linalg_equal_bool(out1, tempssq6, (size_t)(LINALG_SQUARE * LINALG_SQUARE));
    lok(out1);
    lok(out2);
    linalg_all_bool(out1, (size_t)(LINALG_SQUARE * LINALG_SQUARE));

    free(out1);
    free(out2);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.2f);
    out2 = linalg_equal_bool(out1, tempssq6, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.02f);
    out2 = linalg_equal_bool(out1, tempssq3, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.002f);
    out2 = linalg_equal_bool(out1, tempssq4, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.0002f);
    out2 = linalg_equal_bool(out1, tempssq5, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 0.00001f);
    out2 = linalg_equal_bool(out1, tempssq7, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
    free(out1);
    free(out2);
}

enum ENUM_TEST_PATHFINDING {
    ROW_LEN_TEST_PATHFINDING = 21,
    COL_LEN_TEST_PATHFINDING = 25,
};


// void test_q_math() {
//     // dupprintf(globalf, "\ntest_q_math\n");
//     // dupprintf(globalf, "\ntest_q_cycles\n");
//     lok(q_cycle2_zp(0) == 0);
//     lok(q_cycle2_zp(1) == 1);
//     lok(q_cycle2_zp(2) == 0);
//     lok(q_cycle2_zp(3) == 1);
//     lok(q_cycle2_pz(0) == 1);
//     lok(q_cycle2_pz(1) == 0);
//     lok(q_cycle2_pz(2) == 1);
//     lok(q_cycle2_pz(3) == 0);
//     lok(q_cycle2_mz(0) == -1);
//     lok(q_cycle2_mz(1) == 0);
//     lok(q_cycle2_mz(2) == -1);
//     lok(q_cycle2_mz(3) == 0);
//     lok(q_cycle2_zm(0) == 0);
//     lok(q_cycle2_zm(1) == -1);
//     lok(q_cycle2_zm(2) == 0);
//     lok(q_cycle2_zm(3) == -1);
//     lok(q_cycle2_pm(0) == 1);
//     lok(q_cycle2_pm(1) == -1);
//     lok(q_cycle2_pm(2) == 1);
//     lok(q_cycle2_pm(3) == -1);
//     lok(q_cycle2_mp(0) == -1);
//     lok(q_cycle2_mp(1) == 1);
//     lok(q_cycle2_mp(2) == -1);
//     lok(q_cycle2_mp(3) == 1);


//     lok(q_cycle3_mzp(0) == -1);
//     lok(q_cycle3_mzp(1) == 0);
//     lok(q_cycle3_mzp(2) == 1);
//     lok(q_cycle3_mzp(3) == -1);
//     lok(q_cycle3_mzp(4) == 0);
//     lok(q_cycle3_mzp(5) == 1);
//     lok(q_cycle3_pmz(0) == 1);
//     lok(q_cycle3_pmz(1) == -1);
//     lok(q_cycle3_pmz(2) == 0);
//     lok(q_cycle3_pmz(3) == 1);
//     lok(q_cycle3_pmz(4) == -1);
//     lok(q_cycle3_pmz(5) == 0);
//     lok(q_cycle3_zpm(0) == 0);
//     lok(q_cycle3_zpm(1) == 1);
//     lok(q_cycle3_zpm(2) == -1);
//     lok(q_cycle3_zpm(3) == 0);
//     lok(q_cycle3_zpm(4) == 1);
//     lok(q_cycle3_zpm(5) == -1);
//     lok(q_cycle3_mpz(0) == -1);
//     lok(q_cycle3_mpz(1) == 1);
//     lok(q_cycle3_mpz(2) == 0);
//     lok(q_cycle3_mpz(3) == -1);
//     lok(q_cycle3_mpz(4) == 1);
//     lok(q_cycle3_mpz(5) == 0);
//     lok(q_cycle3_pzm(0) == 1);
//     lok(q_cycle3_pzm(1) == 0);
//     lok(q_cycle3_pzm(2) == -1);
//     lok(q_cycle3_pzm(3) == 1);
//     lok(q_cycle3_pzm(4) == 0);
//     lok(q_cycle3_pzm(5) == -1);
//     lok(q_cycle3_zmp(0) == 0);
//     lok(q_cycle3_zmp(1) == -1);
//     lok(q_cycle3_zmp(2) == 1);
//     lok(q_cycle3_zmp(3) == 0);
//     lok(q_cycle3_zmp(4) == -1);
//     lok(q_cycle3_zmp(5) == 1);

//     lok(q_cycle4_mzpz(0) == -1);
//     lok(q_cycle4_mzpz(1) == 0);
//     lok(q_cycle4_mzpz(2) == 1);
//     lok(q_cycle4_mzpz(3) == 0);
//     lok(q_cycle4_mzpz(4) == -1);
//     lok(q_cycle4_mzpz(5) == 0);
//     lok(q_cycle4_mzpz(6) == 1);
//     lok(q_cycle4_mzpz(7) == 0);
//     lok(q_cycle4_pzmz(0) == 1);
//     lok(q_cycle4_pzmz(1) == 0);
//     lok(q_cycle4_pzmz(2) == -1);
//     lok(q_cycle4_pzmz(3) == 0);
//     lok(q_cycle4_pzmz(4) == 1);
//     lok(q_cycle4_pzmz(5) == 0);
//     lok(q_cycle4_pzmz(6) == -1);
//     lok(q_cycle4_pzmz(7) == 0);
//     lok(q_cycle4_zmzp(0) == 0);
//     lok(q_cycle4_zmzp(1) == -1);
//     lok(q_cycle4_zmzp(2) == 0);
//     lok(q_cycle4_zmzp(3) == 1);
//     lok(q_cycle4_zmzp(4) == 0);
//     lok(q_cycle4_zmzp(5) == -1);
//     lok(q_cycle4_zmzp(6) == 0);
//     lok(q_cycle4_zmzp(7) == 1);
//     lok(q_cycle4_zmzp(0) == 0);
//     lok(q_cycle4_zmzp(1) == -1);
//     lok(q_cycle4_zmzp(2) == 0);
//     lok(q_cycle4_zmzp(3) == 1);
//     lok(q_cycle4_zmzp(4) == 0);
//     lok(q_cycle4_zmzp(5) == -1);
//     lok(q_cycle4_zmzp(6) == 0);
//     lok(q_cycle4_zmzp(7) == 1);

//     lok(q_cycle6_mzpzzz(0) == -1);
//     lok(q_cycle6_mzpzzz(1) == 0);
//     lok(q_cycle6_mzpzzz(2) == 1);
//     lok(q_cycle6_mzpzzz(3) == 0);
//     lok(q_cycle6_mzpzzz(4) == 0);
//     lok(q_cycle6_mzpzzz(5) == 0);
//     lok(q_cycle6_mzpzzz(6) == -1);
//     lok(q_cycle6_mzpzzz(7) == 0);
//     lok(q_cycle6_mzpzzz(8) == 1);
//     lok(q_cycle6_mzpzzz(9) == 0);
//     lok(q_cycle6_mzpzzz(10) == 0);
//     lok(q_cycle6_mzpzzz(11) == 0);
//     lok(q_cycle6_zzzmzp(0) == 0);
//     lok(q_cycle6_zzzmzp(1) == 0);
//     lok(q_cycle6_zzzmzp(2) == 0);
//     lok(q_cycle6_zzzmzp(3) == -1);
//     lok(q_cycle6_zzzmzp(4) == 0);
//     lok(q_cycle6_zzzmzp(5) == 1);
//     lok(q_cycle6_zzzmzp(6) == 0);
//     lok(q_cycle6_zzzmzp(7) == 0);
//     lok(q_cycle6_zzzmzp(8) == 0);
//     lok(q_cycle6_zzzmzp(9) == -1);
//     lok(q_cycle6_zzzmzp(10) == 0);
//     lok(q_cycle6_zzzmzp(11) == 1);
//     lok(q_cycle6_zzmzpz(0) == 0);
//     lok(q_cycle6_zzmzpz(1) == 0);
//     lok(q_cycle6_zzmzpz(2) == -1);
//     lok(q_cycle6_zzmzpz(3) == 0);
//     lok(q_cycle6_zzmzpz(4) == 1);
//     lok(q_cycle6_zzmzpz(5) == 0);
//     lok(q_cycle6_zzmzpz(6) == 0);
//     lok(q_cycle6_zzmzpz(7) == 0);
//     lok(q_cycle6_zzmzpz(8) == -1);
//     lok(q_cycle6_zzmzpz(9) == 0);
//     lok(q_cycle6_zzmzpz(10) == 1);
//     lok(q_cycle6_zzmzpz(11) == 0);
//     lok(q_cycle6_zpzzzm(0) == 0);
//     lok(q_cycle6_zpzzzm(1) == 1);
//     lok(q_cycle6_zpzzzm(2) == 0);
//     lok(q_cycle6_zpzzzm(3) == 0);
//     lok(q_cycle6_zpzzzm(4) == 0);
//     lok(q_cycle6_zpzzzm(5) == -1);
//     lok(q_cycle6_zpzzzm(6) == 0);
//     lok(q_cycle6_zpzzzm(7) == 1);
//     lok(q_cycle6_zpzzzm(8) == 0);
//     lok(q_cycle6_zpzzzm(9) == 0);
//     lok(q_cycle6_zpzzzm(10) == 0);
//     lok(q_cycle6_zpzzzm(11) == -1);
//     lok(q_cycle6_zmzpzz(0) == 0);
//     lok(q_cycle6_zmzpzz(1) == -1);
//     lok(q_cycle6_zmzpzz(2) == 0);
//     lok(q_cycle6_zmzpzz(3) == 1);
//     lok(q_cycle6_zmzpzz(4) == 0);
//     lok(q_cycle6_zmzpzz(5) == 0);
//     lok(q_cycle6_zmzpzz(6) == 0);
//     lok(q_cycle6_zmzpzz(7) == -1);
//     lok(q_cycle6_zmzpzz(8) == 0);
//     lok(q_cycle6_zmzpzz(9) == 1);
//     lok(q_cycle6_zmzpzz(10) == 0);
//     lok(q_cycle6_zmzpzz(11) == 0);
//     lok(q_cycle6_pzzzmz(0) == 1);
//     lok(q_cycle6_pzzzmz(1) == 0);
//     lok(q_cycle6_pzzzmz(2) == 0);
//     lok(q_cycle6_pzzzmz(3) == 0);
//     lok(q_cycle6_pzzzmz(4) == -1);
//     lok(q_cycle6_pzzzmz(5) == 0);
//     lok(q_cycle6_pzzzmz(6) == 1);
//     lok(q_cycle6_pzzzmz(7) == 0);
//     lok(q_cycle6_pzzzmz(8) == 0);
//     lok(q_cycle6_pzzzmz(9) == 0);
//     lok(q_cycle6_pzzzmz(10) == -1);
//     lok(q_cycle6_pzzzmz(11) == 0);
//     lok(q_cycle6_mzzzpz(0) == -1);
//     lok(q_cycle6_mzzzpz(1) == 0);
//     lok(q_cycle6_mzzzpz(2) == 0);
//     lok(q_cycle6_mzzzpz(3) == 0);
//     lok(q_cycle6_mzzzpz(4) == 1);
//     lok(q_cycle6_mzzzpz(5) == 0);
//     lok(q_cycle6_mzzzpz(6) == -1);
//     lok(q_cycle6_mzzzpz(7) == 0);
//     lok(q_cycle6_mzzzpz(8) == 0);
//     lok(q_cycle6_mzzzpz(9) == 0);
//     lok(q_cycle6_mzzzpz(10) == 1);
//     lok(q_cycle6_mzzzpz(11) == 0);
//     lok(q_cycle6_zpzmzz(0) == 0);
//     lok(q_cycle6_zpzmzz(1) == 1);
//     lok(q_cycle6_zpzmzz(2) == 0);
//     lok(q_cycle6_zpzmzz(3) == -1);
//     lok(q_cycle6_zpzmzz(4) == 0);
//     lok(q_cycle6_zpzmzz(5) == 0);
//     lok(q_cycle6_zpzmzz(6) == 0);
//     lok(q_cycle6_zpzmzz(7) == 1);
//     lok(q_cycle6_zpzmzz(8) == 0);
//     lok(q_cycle6_zpzmzz(9) == -1);
//     lok(q_cycle6_zpzmzz(10) == 0);
//     lok(q_cycle6_zpzmzz(11) == 0);
//     lok(q_cycle6_pzmzzz(0) == 1);
//     lok(q_cycle6_pzmzzz(1) == 0);
//     lok(q_cycle6_pzmzzz(2) == -1);
//     lok(q_cycle6_pzmzzz(3) == 0);
//     lok(q_cycle6_pzmzzz(4) == 0);
//     lok(q_cycle6_pzmzzz(5) == 0);
//     lok(q_cycle6_pzmzzz(6) == 1);
//     lok(q_cycle6_pzmzzz(7) == 0);
//     lok(q_cycle6_pzmzzz(8) == -1);
//     lok(q_cycle6_pzmzzz(9) == 0);
//     lok(q_cycle6_pzmzzz(10) == 0);
//     lok(q_cycle6_pzmzzz(11) == 0);
//     lok(q_cycle6_zzzpzm(0) == 0);
//     lok(q_cycle6_zzzpzm(1) == 0);
//     lok(q_cycle6_zzzpzm(2) == 0);
//     lok(q_cycle6_zzzpzm(3) == 1);
//     lok(q_cycle6_zzzpzm(4) == 0);
//     lok(q_cycle6_zzzpzm(5) == -1);
//     lok(q_cycle6_zzzpzm(6) == 0);
//     lok(q_cycle6_zzzpzm(7) == 0);
//     lok(q_cycle6_zzzpzm(8) == 0);
//     lok(q_cycle6_zzzpzm(9) == 1);
//     lok(q_cycle6_zzzpzm(10) == 0);
//     lok(q_cycle6_zzzpzm(11) == -1);
//     lok(q_cycle6_zzpzmz(0) == 0);
//     lok(q_cycle6_zzpzmz(1) == 0);
//     lok(q_cycle6_zzpzmz(2) == 1);
//     lok(q_cycle6_zzpzmz(3) == 0);
//     lok(q_cycle6_zzpzmz(4) == -1);
//     lok(q_cycle6_zzpzmz(5) == 0);
//     lok(q_cycle6_zzpzmz(6) == 0);
//     lok(q_cycle6_zzpzmz(7) == 0);
//     lok(q_cycle6_zzpzmz(8) == 1);
//     lok(q_cycle6_zzpzmz(9) == 0);
//     lok(q_cycle6_zzpzmz(10) == -1);
//     lok(q_cycle6_zzpzmz(11) == 0);
//     lok(q_cycle6_zmzzzp(0) == 0);
//     lok(q_cycle6_zmzzzp(1) == -1);
//     lok(q_cycle6_zmzzzp(2) == 0);
//     lok(q_cycle6_zmzzzp(3) == 0);
//     lok(q_cycle6_zmzzzp(4) == 0);
//     lok(q_cycle6_zmzzzp(5) == 1);
//     lok(q_cycle6_zmzzzp(6) == 0);
//     lok(q_cycle6_zmzzzp(7) == -1);
//     lok(q_cycle6_zmzzzp(8) == 0);
//     lok(q_cycle6_zmzzzp(9) == 0);
//     lok(q_cycle6_zmzzzp(10) == 0);
//     lok(q_cycle6_zmzzzp(11) == 1);
//     lok(q_cycle6_mppmzz(0) == -1);
//     lok(q_cycle6_mppmzz(1) == 1);
//     lok(q_cycle6_mppmzz(2) == 1);
//     lok(q_cycle6_mppmzz(3) == -1);
//     lok(q_cycle6_mppmzz(4) == 0);
//     lok(q_cycle6_mppmzz(5) == 0);
//     lok(q_cycle6_mppmzz(6) == -1);
//     lok(q_cycle6_mppmzz(7) == 1);
//     lok(q_cycle6_mppmzz(8) == 1);
//     lok(q_cycle6_mppmzz(9) == -1);
//     lok(q_cycle6_mppmzz(10) == 0);
//     lok(q_cycle6_mppmzz(11) == 0);
//     lok(q_cycle6_zzmppm(0) == 0);
//     lok(q_cycle6_zzmppm(1) == 0);
//     lok(q_cycle6_zzmppm(2) == -1);
//     lok(q_cycle6_zzmppm(3) == 1);
//     lok(q_cycle6_zzmppm(4) == 1);
//     lok(q_cycle6_zzmppm(5) == -1);
//     lok(q_cycle6_zzmppm(6) == 0);
//     lok(q_cycle6_zzmppm(7) == 0);
//     lok(q_cycle6_zzmppm(8) == -1);
//     lok(q_cycle6_zzmppm(9) == 1);
//     lok(q_cycle6_zzmppm(10) == 1);
//     lok(q_cycle6_zzmppm(11) == -1);
//     lok(q_cycle6_pmzzmp(0) == 1);
//     lok(q_cycle6_pmzzmp(1) == -1);
//     lok(q_cycle6_pmzzmp(2) == 0);
//     lok(q_cycle6_pmzzmp(3) == 0);
//     lok(q_cycle6_pmzzmp(4) == -1);
//     lok(q_cycle6_pmzzmp(5) == 1);
//     lok(q_cycle6_pmzzmp(6) == 1);
//     lok(q_cycle6_pmzzmp(7) == -1);
//     lok(q_cycle6_pmzzmp(8) == 0);
//     lok(q_cycle6_pmzzmp(9) == 0);
//     lok(q_cycle6_pmzzmp(10) == -1);
//     lok(q_cycle6_pmzzmp(11) == 1);

//     // dupprintf(globalf, "\nGeometric sequences\n");
//     int32_t current;
//     int32_t upper1;
//     int32_t upper2;
//     int32_t lower;
//     int32_t out;
//     int32_t geo_factor;
//     int32_t distance;
//     current = 0;
//     upper1 = 3;
//     upper2 = 6;
//     lower = 0;
//     distance = 32;
//     geo_factor = 2;
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 16);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 8);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 4);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 2);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 1);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 1);
//     geo_factor = 3;
//     distance = 243;
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 81);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 27);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 9);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 3);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 1);
//     distance = q_sequence_geometric_int32_t(distance, 0, geo_factor);
//     lok(distance == 1);
//     lower = 0;
//     // dupprintf(globalf, "\npingpong sequences\n");
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 0);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 0);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 1);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 1);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 2);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 2);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 1);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 3);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 0);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 4);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 1);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 5);
//     current++;
//     out = q_sequence_pingpong_int32_t(current, upper1, lower);
//     lok(out == 2);
//     out = q_sequence_pingpong_int32_t(current, upper2, lower);
//     lok(out == 4);
// }

// void test_pathfinding() {
//     SDL_Log("test_pathfinding");
//     uint32_t move;
//     struct Point start = {10, 6};
//     struct Point end = {15, 1};
//     struct HexPoint hexstart = {10, -4, 6};
//     int_tile_t range[2];
//     int_tile_t * position;

//     move = 5;
//     range[0] = 1;
//     range[1] = 2;

//     int_tile_t temp_costmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_movemapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_hexmovemap[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 4, 4, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 3, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 3, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 4, 4, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };

//     int_tile_t temp_attackmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_hexattackmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };

//     int_tile_t temp_blockmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_sightmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 3, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 2, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };
//     int_tile_t temp_hexsightmapp[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 3, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 2, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 0, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };

//     SDL_Log("more init matrices");

//     int_tile_t * attackmappp = NULL;
//     int_tile_t * hexattackmappp = NULL;
//     int_tile_t * movemappp = NULL;
//     int_tile_t * blockmappp = NULL;
//     int_tile_t * sightmappp = NULL;
//     int_tile_t * hexmovemappp = NULL;
//     int_tile_t * hexsightmappp = NULL;

//     int_tile_t * costmappp = calloc(sizeof(*costmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemappp = calloc(sizeof(*movemappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     attackmappp = calloc(sizeof(*attackmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     hexattackmappp = calloc(sizeof(*hexattackmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     blockmappp = calloc(sizeof(*blockmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     sightmappp = calloc(sizeof(*sightmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     hexmovemappp = calloc(sizeof(*hexmovemappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     hexsightmappp = calloc(sizeof(*hexsightmappp), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);

//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             attackmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             hexattackmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_hexattackmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             movemappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp[row * COL_LEN_TEST_PATHFINDING + col];
//             costmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_costmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             blockmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_blockmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             sightmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_sightmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             hexsightmappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_hexsightmapp[row * COL_LEN_TEST_PATHFINDING + col];
//             hexmovemappp[row * COL_LEN_TEST_PATHFINDING + col] = temp_hexmovemap[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }

//     int_tile_t current = 0;
//     int_tile_t temp_row = 0;
//     int_tile_t temp_col = 0;
//     int_tile_t * computed_movemapp = Pathfinding_Map_Move(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * computed_movemapp_list = Pathfinding_Map_Move(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_LIST);
//     // printf("DARR_NUM(computed_movemapp_list) %d", DARR_NUM(computed_movemapp_list));
//     lok(DARR_NUM(computed_movemapp_list) == 122);
//     for (uint16_t i = 0; i < DARR_NUM(computed_movemapp_list) / 2; i++) {
//         temp_col = computed_movemapp_list[i * TWO_D + 0];
//         temp_row = computed_movemapp_list[i * TWO_D + 1];
//         current = computed_movemapp[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         // SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current > 0);
//     }

//     int_tile_t * computed_attackmapp = Pathfinding_Map_Attack(movemappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     // matrix_print_int16_t(computed_attackmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     int_tile_t * computed_attackmapp_list = Pathfinding_Map_Attack(movemappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);
//     // printf("DARR_NUM(computed_attackmapp_list) %d", DARR_NUM(computed_attackmapp_list));
//     lok(DARR_NUM(computed_attackmapp_list) == 102);
//     for (uint16_t i = 0; i < DARR_NUM(computed_attackmapp_list) / 2; i++) {
//         temp_col = computed_attackmapp_list[i * TWO_D + 0];
//         temp_row = computed_attackmapp_list[i * TWO_D + 1];
//         current = computed_attackmapp[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         // SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current == 1);
//     }
//     int16_t * out = NULL;
//     out = linalg_equal_int16_t(computed_movemapp, movemappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);

//     out = linalg_equal_int16_t(computed_attackmapp, attackmappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(computed_attackmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // linalg_matrix_print_int16_t(attackmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     int_tile_t temp_costmapp2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_movemapp2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_hexmovemapp2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 4, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 5, 5, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };
//     int_tile_t temp_attackmapp2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     int_tile_t temp_hexattackmapp2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };
//     SDL_Log("more init matrices AGAIN ");

//     int_tile_t * attackmappp2 = NULL;
//     int_tile_t * hexattackmappp2 = NULL;
//     int_tile_t * movemappp2 = NULL;
//     int_tile_t * hexmovemappp2 = NULL;
//     int_tile_t * costmappp2 = NULL;
//     costmappp2 = calloc(sizeof(*costmappp2), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemappp2 = calloc(sizeof(*movemappp2), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     attackmappp2 = calloc(sizeof(*attackmappp2), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     hexattackmappp2 = calloc(sizeof(*hexattackmappp2), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     hexmovemappp2 = calloc(sizeof(*hexmovemappp2), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);

//     SDL_Log("init loop ");
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             attackmappp2[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp2[row * COL_LEN_TEST_PATHFINDING + col];
//             movemappp2[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp2[row * COL_LEN_TEST_PATHFINDING + col];
//             costmappp2[row * COL_LEN_TEST_PATHFINDING + col] = temp_costmapp2[row * COL_LEN_TEST_PATHFINDING + col];
//             hexmovemappp2[row * COL_LEN_TEST_PATHFINDING + col] = temp_hexmovemapp2[row * COL_LEN_TEST_PATHFINDING + col];
//             hexattackmappp2[row * COL_LEN_TEST_PATHFINDING + col] = temp_hexattackmapp2[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }
//     SDL_Log("Pathfinding_Map_Move test AGAIN ");

//     int_tile_t * computed_movemapp2 = Pathfinding_Map_Move(costmappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * computed_movemapp_list2 = Pathfinding_Map_Move(costmappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_LIST);
//     for (uint16_t i = 0; i < DARR_NUM(computed_movemapp_list2) / 2; i++) {
//         temp_col = computed_movemapp_list2[i * TWO_D + 0];
//         temp_row = computed_movemapp_list2[i * TWO_D + 1];
//         current = computed_movemapp2[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current > 0);
//     }
//     int_tile_t * computed_attackmapp2 = Pathfinding_Map_Attack(movemappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     int_tile_t * computed_attackmapp_list2 = Pathfinding_Map_Attack(movemappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);

//     for (uint16_t i = 0; i < DARR_NUM(computed_attackmapp_list2) / 2; i++) {
//         temp_col = computed_attackmapp_list2[i * TWO_D + 0];
//         temp_row = computed_attackmapp_list2[i * TWO_D + 1];
//         current = computed_attackmapp2[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current == 1);
//     }

//     out = linalg_equal_int16_t(computed_movemapp2, movemappp2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     out = linalg_equal_int16_t(computed_attackmapp2, attackmappp2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);

//     int_tile_t * computed_sightmapp = Pathfinding_Map_Sight(blockmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, 6, POINTS_MATRIX);
//     SDL_Log("Pathfinding_Map_Sight OUT");

//     out = linalg_equal_int16_t(computed_sightmapp, sightmappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(blockmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // linalg_matrix_print_int16_t(computed_sightmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // linalg_matrix_print_int16_t(sightmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     SDL_Log("Pathfinding_Map_Move_Hex IN");
//     int_tile_t * computed_hexmovemapp = Pathfinding_Map_Move_Hex(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, hexstart, move, POINTS_MATRIX);
//     int_tile_t * computed_hexmovemapp_list = Pathfinding_Map_Move_Hex(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, hexstart, move, POINTS_LIST);
//     SDL_Log("Pathfinding_Map_Move_Hex out");
//     out = linalg_equal_int16_t(computed_hexmovemapp, hexmovemappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     for (uint16_t i = 0; i < DARR_NUM(computed_hexmovemapp_list) / 2; i++) {
//         temp_col = computed_hexmovemapp_list[i * TWO_D + 0];
//         temp_row = computed_hexmovemapp_list[i * TWO_D + 1];
//         current = computed_hexmovemapp[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current > 0);
//     }

//     int_tile_t * computed_hexmovemapp2 = Pathfinding_Map_Move_Hex(costmappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, hexstart, move, POINTS_MATRIX);
//     int_tile_t * computed_hexmovemapp_list2 = Pathfinding_Map_Move_Hex(costmappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, hexstart, move, POINTS_LIST);
//     out = linalg_equal_int16_t(computed_hexmovemapp2, hexmovemappp2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     for (uint16_t i = 0; i < DARR_NUM(computed_hexmovemapp_list2) / 2; i++) {
//         temp_col = computed_hexmovemapp_list2[i * TWO_D + 0];
//         temp_row = computed_hexmovemapp_list2[i * TWO_D + 1];
//         current = computed_hexmovemapp2[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current > 0);
//     }


//     int_tile_t * computed_hexattackmapp = Pathfinding_Map_Attack(hexmovemappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     int_tile_t * computed_hexattackmapp_list = Pathfinding_Map_Attack(hexmovemappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);
//     lok(linalg_equal_int16_t(computed_hexattackmapp, hexattackmappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     for (uint16_t i = 0; i < DARR_NUM(computed_hexattackmapp_list) / 2; i++) {
//         // SDL_Log("i:%d %d %d %d", i, temp_row, temp_col, current);
//         temp_col = computed_hexattackmapp_list[i * TWO_D + 0];
//         temp_row = computed_hexattackmapp_list[i * TWO_D + 1];
//         current = computed_hexattackmapp[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }

//     int_tile_t * computed_hexattackmapp2 = Pathfinding_Map_Attack(hexmovemappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     out = linalg_equal_int16_t(computed_hexattackmapp2, hexattackmappp2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * computed_hexattackmapp_list2 = Pathfinding_Map_Attack(hexmovemappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);
//     for (uint16_t i = 0; i < DARR_NUM(computed_hexattackmapp_list2) / 2; i++) {
//         temp_col = computed_hexattackmapp_list2[i * TWO_D + 0];
//         temp_row = computed_hexattackmapp_list2[i * TWO_D + 1];
//         current = computed_hexattackmapp2[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }
//     // plotHex2DArray(hexmovemappp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, POINTY_TOP);
//     // plotHex2DArray(computed_hexattackmapp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, POINTY_TOP);
//     int_tile_t * computed_hexsightmapp = Pathfinding_Map_Sight_Hex(blockmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, hexstart, 6, POINTS_MATRIX);
//     // int_tile_t * computed_hexsightmapp = Pathfinding_Map_Sight(blockmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, 6, POINTS_MATRIX);
//     out = linalg_equal_int16_t(computed_hexsightmapp, hexsightmappp, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // plotHex2DArray(computed_hexsightmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, POINTY_TOP);
//     // lok(linalg_equal_int16_t(computed_sightmapp, sightmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING));



//     // linalg_matrix_print_int16_t(computed_sightmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // plotHex2DArray(computed_sightmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, FLAT_TOP);
//     // plotHex2DArray(computed_sightmapp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, POINTY_TOP);

//     move = 1;
//     range[0] = 5;
//     range[1] = 6;

//     int_tile_t temp_attackmapp3_include[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_attackmapp3_exclude[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };


//     int_tile_t temp_movemapp3[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t * movemappp3 = NULL;
//     int_tile_t * attackmapp3_exclude = NULL;
//     int_tile_t * attackmapp3_include = NULL;

//     SDL_Log("more init matrices AGAIN V2 ");

//     attackmapp3_include = calloc(sizeof(*attackmapp3_include), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     attackmapp3_exclude = calloc(sizeof(*attackmapp3_exclude), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemappp3 = calloc(sizeof(*movemappp3), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             movemappp3[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp3[row * COL_LEN_TEST_PATHFINDING + col];
//             attackmapp3_exclude[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp3_exclude[row * COL_LEN_TEST_PATHFINDING + col];
//             attackmapp3_include[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp3_include[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }

//     int_tile_t * computed_movemapp3 = Pathfinding_Map_Move(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * computed_movemapp3_list = Pathfinding_Map_Move(costmappp, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_LIST);
//     for (uint16_t i = 0; i < DARR_NUM(computed_movemapp3_list) / 2; i++) {
//         temp_col = computed_movemapp3_list[i * TWO_D + 0];
//         temp_row = computed_movemapp3_list[i * TWO_D + 1];
//         current = computed_movemapp3[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current > 0);
//     }

//     int_tile_t * computed_attackmapp3_exclude = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     out = linalg_equal_int16_t(computed_attackmapp3_exclude, attackmapp3_exclude, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * attackmapp3_exclude_list = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);
//     for (uint16_t i = 0; i < DARR_NUM(attackmapp3_exclude_list) / 2; i++) {
//         temp_col = attackmapp3_exclude_list[i * TWO_D + 0];
//         temp_row = attackmapp3_exclude_list[i * TWO_D + 1];
//         current = computed_attackmapp3_exclude[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }

//     int_tile_t * computed_attackmapp3_include = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_INCLUDE);
//     // linalg_matrix_print_int16_t(computed_attackmapp3_include, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     out = linalg_equal_int16_t(computed_attackmapp3_include, attackmapp3_exclude, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * attackmapp3_include_list = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_INCLUDE);
//     for (uint16_t i = 0; i < DARR_NUM(attackmapp3_exclude_list) / 2; i++) {
//         temp_col = attackmapp3_include_list[i * TWO_D + 0];
//         temp_row = attackmapp3_include_list[i * TWO_D + 1];
//         current = computed_attackmapp3_include[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }

//     move = 1;
//     range[0] = 1;
//     range[1] = 6;

//     int_tile_t temp_attackmapp4_exclude[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };


//     int_tile_t temp_attackmapp4_include[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };


//     int_tile_t * attackmapp4_exclude = NULL;
//     int_tile_t * attackmapp4_include = NULL;
//     SDL_Log("more init matrices AGAIN V3 ");
//     attackmapp4_include = calloc(sizeof(*attackmapp4_include), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     attackmapp4_exclude = calloc(sizeof(*attackmapp4_exclude), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             attackmapp4_exclude[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp4_exclude[row * COL_LEN_TEST_PATHFINDING + col];
//             attackmapp4_include[row * COL_LEN_TEST_PATHFINDING + col] = temp_attackmapp4_include[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }

//     int_tile_t * computed_attackmapp4_exclude = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
//     // linalg_matrix_print_int16_t(computed_attackmapp4_exclude, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(computed_attackmapp4_exclude, attackmapp4_exclude, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * attackmapp4_exclude_list = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_EXCLUDE);
//     for (uint16_t i = 0; i < DARR_NUM(attackmapp4_exclude_list) / 2; i++) {
//         temp_col = attackmapp4_exclude_list[i * TWO_D + 0];
//         temp_row = attackmapp4_exclude_list[i * TWO_D + 1];
//         current = computed_attackmapp4_exclude[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }

//     int_tile_t * computed_attackmapp4_include = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_MATRIX, MOVETILE_INCLUDE);
//     // linalg_matrix_print_int16_t(computed_attackmapp4_include, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(computed_attackmapp4_include, attackmapp4_include, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * attackmapp4_include_list = Pathfinding_Map_Attack(temp_movemapp3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, move, range, POINTS_LIST, MOVETILE_INCLUDE);
//     SDL_Log("Pathfinding_Map_Attack V3 OUt ");

//     for (uint16_t i = 0; i < DARR_NUM(attackmapp4_include_list) / 2; i++) {
//         temp_col = attackmapp4_include_list[i * TWO_D + 0];
//         temp_row = attackmapp4_include_list[i * TWO_D + 1];
//         current = computed_attackmapp4_include[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current == 1);
//     }

//     move = 30;
//     int_tile_t temp_costmapp5[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_movemapp5[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  8, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
//         16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  7, 0,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
//         15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  6, 0,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//         16, 15, 14, 13, 12, 11, 10, 10,  6,  5,  4,  5, 0,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         17, 16, 15, 14, 13, 12, 11, 12,  8,  5,  3,  4, 0,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
//         18, 17, 16, 15, 14, 13, 12, 11,  7,  4,  2,  3, 4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
//         17, 16, 15, 14, 13, 12, 11, 10,  6,  3,  1,  2, 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
//         18, 17, 16, 15, 14, 13, 12, 11,  7,  4,  2,  3, 4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
//         19, 18, 17, 16, 15, 14, 13, 12,  8,  5,  3,  4, 5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
//         18, 17, 16, 15, 14, 13, 12, 13,  9,  6,  4,  5, 0,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         17, 16, 15, 14, 13, 12, 11, 13, 10,  7,  5,  6, 0,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//         16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  7, 0,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
//         17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  8, 0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
//         18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  9, 0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
//         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9, 10, 0, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
//         20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 11, 0, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
//         21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 12, 0, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
//         22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 13, 0, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
//         23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 14, 0, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
//         24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 15, 0, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
//         25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 16, 0, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
//     };

//     int_tile_t * costmapp5 = NULL;
//     int_tile_t * movemapp5 = NULL;
//     SDL_Log("more init matrices AGAIN V4 ");
//     costmapp5 = calloc(sizeof(*costmapp5), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemapp5 = calloc(sizeof(*movemapp5), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             costmapp5[row * COL_LEN_TEST_PATHFINDING + col] = temp_costmapp5[row * COL_LEN_TEST_PATHFINDING + col];
//             movemapp5[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp5[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }

//     int_tile_t * computed_movemapp5 = Pathfinding_Map_Move(costmapp5, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * computed_movemapp5_list = Pathfinding_Map_Move(costmapp5, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_LIST);
//     SDL_Log("Pathfinding_Map_Move AGAIN V4  OUT");
//     for (uint16_t i = 0; i < DARR_NUM(computed_movemapp5_list) / 2; i++) {
//         temp_col = computed_movemapp5_list[i * TWO_D + 0];
//         temp_row = computed_movemapp5_list[i * TWO_D + 1];
//         current = computed_movemapp5[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         // SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current > 0);
//     }
//     // linalg_matrix_print_int16_t(computed_movemapp5, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(movemapp5, computed_movemapp5, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);

//     int_tile_t temp_costmapp6[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_path6[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };

//     int_tile_t temp_movemapp6[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         27, 26, 25, 24, 23, 22, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0, 0,  0, 0, 0,
//         26, 25, 24, 23, 22, 21, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0, 0,  0, 0, 0,
//         25, 24, 23, 22, 21, 20, 19, 20, 21, 22, 23,  0,  0, 28, 29, 30, 31,  0,  0,  0,  0, 0,  0, 0, 0,
//         24, 23, 22, 21, 20, 19, 18, 21,  0,  0,  0,  5,  0, 29, 30, 31,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         23, 22, 21, 20, 19, 18, 17, 17,  8,  5,  3,  4,  0, 30, 31,  0,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         22, 21, 20, 19, 18, 17, 16, 16,  7,  4,  2,  3,  0, 31,  0,  0,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         21, 20, 19, 18, 17, 16, 15, 15,  6,  3,  1,  2,  0, 30, 31,  0,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         20, 19, 18, 17, 16, 15, 14, 16,  7,  4,  2,  3,  0, 29, 30, 31,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         19, 18, 17, 16, 15, 14, 13, 17,  8,  5,  3,  4,  0, 28, 29, 30, 31,  0,  0,  0,  0, 0,  0, 0, 0,
//         18, 17, 16, 15, 14, 13, 12, 13,  9,  6,  4,  5,  0, 27, 28, 29, 30, 31,  0,  0,  0, 0,  0, 0, 0,
//         17, 16, 15, 14, 13, 12, 11, 10, 10,  7,  5,  6,  0, 26, 27, 28, 29, 30, 31,  0,  0, 0,  0, 0, 0,
//         16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  7,  0, 25, 26, 27, 28, 29, 30, 31,  0, 0,  0, 0, 0,
//         17, 16, 15, 14, 13,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25, 26, 27, 28, 29, 30, 31, 0,  0, 0, 0,
//         18, 17, 16, 15, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0, 0, 0,
//         19, 18, 17, 16, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0,  0, 0, 0,
//         20, 19, 18, 17, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0, 0,  0, 0, 0,
//         21, 20, 19, 18, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0, 0,  0, 0, 0,
//         22, 21, 20, 19, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0, 0,  0, 0, 0,
//         23, 22, 21, 20, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0, 0,  0, 0, 0,
//         24, 23, 22, 21, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//         25, 24, 23, 22, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0,  0, 0,  0, 0, 0,
//     };

//     int_tile_t * costmapp6 = NULL;
//     int_tile_t * movemapp6 = NULL;
//     SDL_Log("more init matrices AGAIN V5 ");
//     costmapp6 = calloc(sizeof(*costmapp6), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemapp6 = calloc(sizeof(*movemapp6), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             costmapp6[row * COL_LEN_TEST_PATHFINDING + col] = temp_costmapp6[row * COL_LEN_TEST_PATHFINDING + col];
//             movemapp6[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp6[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }

//     int_tile_t * computed_movemapp6 = Pathfinding_Map_Move(costmapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * computed_movemapp6_list = Pathfinding_Map_Move(costmapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_LIST);
//     for (uint16_t i = 0; i < DARR_NUM(computed_movemapp6_list) / TWO_D; i++) {
//         temp_col = computed_movemapp6_list[i * TWO_D + 0];
//         temp_row = computed_movemapp6_list[i * TWO_D + 1];
//         current = computed_movemapp6[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         // SDL_Log("%d %d %d", temp_row, temp_col, current);
//         lok(current > 0);
//     }
//     // linalg_matrix_print_int16_t(computed_movemapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(movemapp6, computed_movemapp6, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(temp_costmapp2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     // linalg_matrix_print_int16_t(computed_movemapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     int_tile_t * path_list_position6 = Pathfinding_Map_Path(computed_movemapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, end, PATH_POSITION);
//     int_tile_t * path_list_steps6 = Pathfinding_Map_Path(computed_movemapp6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, end, PATH_STEP);
//     int_tile_t * path_list_positionfromsteps6 = Pathfinding_Path_step2position(path_list_steps6, DARR_NUM(path_list_steps6) / TWO_D, start);

//     lok((DARR_NUM(path_list_position6) > 0));
//     lok((DARR_NUM(path_list_steps6) > 0));
//     lok((DARR_NUM(path_list_positionfromsteps6) > 0));

//     int_tile_t * path_matrix_fromposition6 = linalg_list2matrix_int16_t(path_list_position6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, DARR_NUM(path_list_position6) / TWO_D);
//     int_tile_t * path_matrix_fromsteps6 = linalg_list2matrix_int16_t(path_list_positionfromsteps6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, DARR_NUM(path_list_positionfromsteps6) / TWO_D);
//     // int_tile_t * path_matrix6 = linalg_list2matrix_int16_t(path_list_steps6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, abs(end.x - start.x) + abs(end.y - start.y));
//     // linalg_matrix_print_int16_t(path_matrix_fromsteps6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // linalg_matrix_print_int16_t(path_matrix_fromposition6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     out = linalg_equal_int16_t(path_matrix_fromposition6, temp_path6, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     out = linalg_equal_int16_t(path_matrix_fromsteps6, temp_path6, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);

//     int_tile_t temp_costmapp7[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_path7[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     };


//     int_tile_t * costmapp7 = NULL;
//     int_tile_t * movemapp7 = NULL;
//     SDL_Log("more init matrices AGAIN V8 ");
//     costmapp7 = calloc(sizeof(*costmapp7), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     movemapp7 = calloc(sizeof(*movemapp7), COL_LEN_TEST_PATHFINDING * ROW_LEN_TEST_PATHFINDING);
//     for (uint16_t row = 0; row < ROW_LEN_TEST_PATHFINDING; row++) {
//         for (uint16_t col = 0; col < COL_LEN_TEST_PATHFINDING; col++) {
//             costmapp7[row * COL_LEN_TEST_PATHFINDING + col] = temp_costmapp7[row * COL_LEN_TEST_PATHFINDING + col];
//             movemapp7[row * COL_LEN_TEST_PATHFINDING + col] = temp_movemapp6[row * COL_LEN_TEST_PATHFINDING + col];
//         }
//     }
//     int_tile_t * computed_movemapp7 = Pathfinding_Map_Move(costmapp7, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, move, POINTS_MATRIX);
//     int_tile_t * path_list_position7 = Pathfinding_Map_Path(computed_movemapp7, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, end, PATH_POSITION);
//     int_tile_t * path_matrix_fromposition7 = linalg_list2matrix_int16_t(path_list_position7, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, DARR_NUM(path_list_position7) / TWO_D);
//     // linalg_matrix_print_int16_t(path_matrix_fromposition7, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(path_matrix_fromposition7, temp_path7, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t temp_movemap10_include[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         9, 9, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     struct Square_Neighbors temp_blockdistance;
//     temp_blockdistance = Pathfinding_Direction_Block(temp_movemap10_include, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start);
//     lok(temp_blockdistance.bottom == ROW_LEN_TEST_PATHFINDING - start.y - 1);
//     lok(temp_blockdistance.top == start.y);
//     lok(temp_blockdistance.right == COL_LEN_TEST_PATHFINDING - start.x - 1);
//     lok(temp_blockdistance.left == start.x);

//     int_tile_t temp_movemap11_include[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         9, 9, 9, 8, 7, 6, 5, 4, 0, 2, 1, 2, 0, 4, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     temp_blockdistance = Pathfinding_Direction_Block(temp_movemap11_include, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start);
//     lok(temp_blockdistance.bottom == 2);
//     lok(temp_blockdistance.top == 2);
//     lok(temp_blockdistance.right == 2);
//     lok(temp_blockdistance.left == 2);


//     int_tile_t temp_movemap12_include[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         9, 9, 9, 8, 7, 0, 0, 4, 3, 2, 1, 2, 0, 0, 5, 6, 7, 8, 9, 9, 9, 9, 9, 9, 9,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     temp_blockdistance = Pathfinding_Direction_Block(temp_movemap12_include, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start);
//     lok(temp_blockdistance.bottom == 1);
//     lok(temp_blockdistance.top == 3);
//     lok(temp_blockdistance.right == 2);
//     lok(temp_blockdistance.left == 4);

//     int_tile_t temp_movemapp20[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_assailablemap1[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     int_tile_t temp_assailablemap2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };

//     struct Point target = {13, 6};
//     int_tile_t range_1[2] = {1, 2};
//     int_tile_t range_2[2] = {1, 3};
//     // linalg_matrix_print_int16_t(temp_movemapp20, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     int_tile_t * compute_assailablemap1 = Pathfinding_Map_Assailable(temp_movemapp20, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, target, range_1, POINTS_MATRIX);
//     out = linalg_equal_int16_t(compute_assailablemap1, temp_assailablemap1, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * compute_assailablemap2 = Pathfinding_Map_Assailable(temp_movemapp20, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, target, range_2, POINTS_MATRIX);
//     // linalg_matrix_print_int16_t(compute_assailablemap2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     out = linalg_equal_int16_t(compute_assailablemap2, temp_assailablemap2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     int_tile_t * compute_assailablemap2_list = Pathfinding_Map_Assailable(temp_movemapp20, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, target, range_2, POINTS_LIST);
//     lok((DARR_NUM(compute_assailablemap2_list) / TWO_D) == 4);
//     for (uint16_t i = 0; i < DARR_NUM(compute_assailablemap2_list) / TWO_D; i++) {
//         temp_col = compute_assailablemap2_list[i * TWO_D + 0];
//         temp_row = compute_assailablemap2_list[i * TWO_D + 1];
//         current = compute_assailablemap2[temp_row * COL_LEN_TEST_PATHFINDING + temp_col];
//         lok(current > 0);
//     }

//     int_tile_t temp_assailablemap3[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     int_tile_t range_push[2];
//     int_tile_t range_pull[2];

//     range_push[0] = 1;
//     range_push[1] = 2;
//     range_pull[0] = 2;
//     range_pull[1] = 3;

//     struct Square_Neighbors neighbors_pushable = Pathfinding_Direction_Pushable(temp_assailablemap3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_push, start);
//     lok(neighbors_pushable.top == true);
//     lok(neighbors_pushable.bottom == true);
//     lok(neighbors_pushable.right == true);
//     lok(neighbors_pushable.left == false);
//     struct Square_Neighbors neighbors_pullable = Pathfinding_Direction_Pullable(temp_assailablemap3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_pull, start);
//     lok(neighbors_pullable.top == false);
//     lok(neighbors_pullable.bottom == false);
//     lok(neighbors_pullable.right == false);
//     lok(neighbors_pullable.left == false);

//     int_tile_t temp_assailablemap4[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     neighbors_pushable = Pathfinding_Direction_Pushable(temp_assailablemap4, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_push, start);
//     lok(neighbors_pushable.top == true);
//     lok(neighbors_pushable.bottom == true);
//     lok(neighbors_pushable.right == true);
//     lok(neighbors_pushable.left == false);
//     neighbors_pullable = Pathfinding_Direction_Pullable(temp_assailablemap4, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_pull, start);
//     lok(neighbors_pullable.top == true);
//     lok(neighbors_pullable.bottom == true);
//     lok(neighbors_pullable.right == false);
//     lok(neighbors_pullable.left == true);

//     int_tile_t temp_assailablemap5[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     neighbors_pushable = Pathfinding_Direction_Pushable(temp_assailablemap5, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_push, start);
//     lok(neighbors_pushable.top == true);
//     lok(neighbors_pushable.bottom == true);
//     lok(neighbors_pushable.right == true);
//     lok(neighbors_pushable.left == false);
//     neighbors_pullable = Pathfinding_Direction_Pullable(temp_assailablemap5, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_pull, start);
//     lok(neighbors_pullable.top == true);
//     lok(neighbors_pullable.bottom == true);
//     lok(neighbors_pullable.right == false);
//     lok(neighbors_pullable.left == true);

//     int_tile_t temp_assailablemap6[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//     };
//     neighbors_pushable = Pathfinding_Direction_Pushable(temp_assailablemap6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_push, start);
//     lok(neighbors_pushable.top == false);
//     lok(neighbors_pushable.bottom == true);
//     lok(neighbors_pushable.right == true);
//     lok(neighbors_pushable.left == true);
//     neighbors_pullable = Pathfinding_Direction_Pullable(temp_assailablemap6, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, range_pull, start);
//     lok(neighbors_pullable.top == true);
//     lok(neighbors_pullable.bottom == false);
//     lok(neighbors_pullable.right == true);
//     lok(neighbors_pullable.left == true);
//     SDL_Log("Pathfinding_Direction_Pullable OUT");


//     int_tile_t temp_costmap30[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
//     };

//     int_tile_t temp_gradientmap1[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         2, 1, 2, 3, 4, 5,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
//         1, 0, 1, 2, 3, 4,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
//         2, 1, 2, 3, 4, 3,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
//         3, 2, 3, 4, 3, 2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//         4, 3, 4, 3, 2, 1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         5, 4, 4, 3, 2, 2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//         5, 4, 3, 2, 1, 2,  2,  3,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         4, 3, 2, 1, 0, 1,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
//         5, 4, 3, 2, 1, 2,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
//         6, 5, 4, 3, 2, 3,  3,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
//         6, 5, 5, 4, 3, 4,  4,  3,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//         5, 4, 5, 5, 4, 5,  5,  4,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//         4, 3, 4, 5, 5, 6,  6,  5,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
//         3, 2, 3, 4, 5, 6,  7,  6,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
//         2, 1, 2, 3, 4, 5,  6,  7,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
//         1, 0, 1, 2, 3, 4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
//         2, 1, 2, 3, 4, 5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
//         3, 2, 3, 4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
//         4, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
//         5, 4, 5, 6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
//         6, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
//     };

//     struct Point * units = NULL;
//     units = calloc(sizeof(*units), 5);
//     units[0].x = 1;
//     units[0].y = 1;
//     units[1].x = 6;
//     units[1].y = 4;
//     units[2].x = 4;
//     units[2].y = 7;
//     units[3].x = 8;
//     units[3].y = 8;
//     units[4].x = 1;
//     units[4].y = 15;
//     int_tile_t * computed_gradientmap1 = Pathfinding_Map_unitGradient(temp_costmap30, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, units, 5);
//     SDL_Log("Pathfinding_Map_unitGradient OUT");
//     out = linalg_equal_int16_t(computed_gradientmap1, temp_gradientmap1, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(computed_gradientmap1, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
//     // linalg_matrix_print_int16_t(temp_gradientmap1, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     int_tile_t temp_pushpullable1[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1,  3,  2,  1,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
//         };


//     temp_blockdistance.right = 2;
//     temp_blockdistance.top = 3;
//     temp_blockdistance.left = 4;
//     temp_blockdistance.bottom = 5;

//     neighbors_pullable.top = true;
//     neighbors_pullable.bottom = true;
//     neighbors_pullable.right = true;
//     neighbors_pullable.left = true;

//     int_tile_t * computed_pushpullable1 = Pathfinding_Map_PushPullable(temp_blockdistance, neighbors_pullable, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, POINTS_MATRIX);
//     out = linalg_equal_int16_t(computed_pushpullable1, temp_pushpullable1, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(computed_pushpullable1, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     int_tile_t temp_pushpullable2[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1,  3,  2,  1,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
//         };


//     temp_blockdistance.right = 2;
//     temp_blockdistance.top = 3;
//     temp_blockdistance.left = 4;
//     temp_blockdistance.bottom = 5;

//     neighbors_pullable.top = false;
//     neighbors_pullable.bottom = true;
//     neighbors_pullable.right = true;
//     neighbors_pullable.left = true;

//     int_tile_t * computed_pushpullable2 = Pathfinding_Map_PushPullable(temp_blockdistance, neighbors_pullable, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, POINTS_MATRIX);
//     out = linalg_equal_int16_t(computed_pushpullable2, temp_pushpullable2, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(computed_pushpullable2, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);

//     int_tile_t temp_pushpullable3[ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING] = {
//         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1,  3,  2,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//             -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
//         };


//     temp_blockdistance.right = 2;
//     temp_blockdistance.top = 3;
//     temp_blockdistance.left = 4;
//     temp_blockdistance.bottom = 5;

//     neighbors_pullable.top = false;
//     neighbors_pullable.bottom = true;
//     neighbors_pullable.right = false;
//     neighbors_pullable.left = true;

//     int_tile_t * computed_pushpullable3 = Pathfinding_Map_PushPullable(temp_blockdistance, neighbors_pullable, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING, start, POINTS_MATRIX);
//     out = linalg_equal_int16_t(computed_pushpullable3, temp_pushpullable3, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING);
//     lok(linalg_all_int16_t(out, ROW_LEN_TEST_PATHFINDING * COL_LEN_TEST_PATHFINDING));
//     free(out);
//     // linalg_matrix_print_int16_t(computed_pushpullable3, ROW_LEN_TEST_PATHFINDING, COL_LEN_TEST_PATHFINDING);
// }


int main() {
    globalf = fopen("linalg_test_results.txt", "w+");
    dupprintf(globalf, "\nHello, World! I am testing noursmath.\n");
    lrun("log2", test_log2);
    // lrun("test_pathfinding", test_pathfinding);
    // lrun("test_q_math", test_q_math);
    lrun("test_double", linalg_double);
    // lrun("test_float", linalg_float);
// #define REGISTER_ENUM(type) lrun(STRINGIFY(linalg_##type), linalg_##type);
//     TEMPLATE_TYPES_INT
// #undef REGISTER_ENUM
    lresults();

    dupprintf(globalf, "noursmath Test End \n \n");
    fclose(globalf);
    return (0);
}