#pragma once
#include "kernel/fd_types.h"

#if (!defined FPRINTF)
//内核态是没有定义 fprintf 的，只有 printf，所以重新定义 printf
#define fprintf(_stream, _fmt, ...) printf(_fmt, ##__VA_ARGS__)
#endif

#ifdef FDEBUG
#define try(_expr, _act)
    {                                                              \
        if ((_expr) < 0) {                                                   \
            fprintf(STDERR, "try: %s failed, at line %d, file %s\n", #_expr, \
                    __LINE__, __FILE__);                                     \
            _act;                                                            \
        }                                                                    \
    }

#else
#define try(_expr, _act)
#endif

#ifdef FDEBUG
#define DEBUG(fmt, ...) fprintf(STDERR, fmt, ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...)
#endif

void dbg_arr_i32(int arr[], int st, int ed) {
#ifdef FDEBUG
    for (int i = st; i <= ed; i++) {
        DEBUG("%d ", arr[i]);
    }
    DEBUG("\n");
#endif
}
