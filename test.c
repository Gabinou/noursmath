#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
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

void test_uint8_t() {
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

void test_int8_t() {
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

void test_uint16_t() {
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

void test_int16_t() {
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

void test_uint32_t() {
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

void test_int32_t() {
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

void test_uint64_t() {
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

void test_int64_t() {
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

void test_bool() {
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

void test_float() {
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

void test_double() {
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

    // matrix_print_bool(out1, LINALG_SQUARE, LINALG_SQUARE);
    // matrix_print_bool(out2, LINALG_SQUARE, LINALG_SQUARE);
    out1 = linalg_equal_double(tempssq1, tempssq2, LINALG_SQUARE * LINALG_SQUARE, 1.0f);
    out2 = linalg_equal_bool(out1, tempssq6, LINALG_SQUARE * LINALG_SQUARE);
    lok(linalg_all_bool(out2, LINALG_SQUARE * LINALG_SQUARE));
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

int main() {
    globalf = fopen("linalg_test_results.txt", "w+");
    dupprintf(globalf, "\nHello, World! I am testing noursmath.\n");
    lrun("log2", test_log2);
    lrun("test_double", test_double);
    lrun("test_float", test_float);
#define REGISTER_ENUM(type) lrun(STRINGIFY(test_##type), test_##type);
    TEMPLATE_TYPES_INT
#undef REGISTER_ENUM
    lresults();

    dupprintf(globalf, "noursmath Test End \n \n");
    fclose(globalf);
    return (0);
}