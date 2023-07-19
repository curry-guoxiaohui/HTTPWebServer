/**
 * @file macro.h
 * @brief 常用宏的封装
 * @author gxh
 */
#ifndef __GXH_MACRO_H__
#define __GXH_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#define GXH_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#define GXH_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define GXH_LIKELY(x) (x)
#define GXH_UNLIKELY(x) (x)
#endif

/// 断言宏封装
#define GXH_ASSERT(x)                                                                \
    if (GXH_UNLIKELY(!(x))) {                                                        \
        GXH_LOG_ERROR(GXH_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\nbacktrace:\n"                          \
                                          << gxh::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

/// 断言宏封装
#define GXH_ASSERT2(x, w)                                                            \
    if (GXH_UNLIKELY(!(x))) {                                                        \
        GXH_LOG_ERROR(GXH_LOG_ROOT()) << "ASSERTION: " #x                          \
                                          << "\n"                                      \
                                          << w                                         \
                                          << "\nbacktrace:\n"                          \
                                          << gxh::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                     \
    }

#endif
