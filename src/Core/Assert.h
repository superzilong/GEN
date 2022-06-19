#pragma once
#include "Log.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifndef NDEBUG
#   define GEN_ASSERT(condition, ...) \
    do { \
        if (! (condition)) { \
            GEN_LOG_ERROR("Assertion `{0}` failed in {1} {2} {3}: {4}", #condition, __FILENAME__ \
                      , " line ", __LINE__, fmt::format(__VA_ARGS__)); \
            __debugbreak(); \
        } \
    } while (false)
#else
#   define GEN_ASSERT(condition, ...) \
	    do { \
        if (! (condition)) { \
            GEN_LOG_ERROR("Assertion `{0}` failed in {1} {2} {3}: {4}", #condition, __FILENAME__ \
                      , " line ", __LINE__, fmt::format(__VA_ARGS__)); \
        } \
    } while (false)
#endif