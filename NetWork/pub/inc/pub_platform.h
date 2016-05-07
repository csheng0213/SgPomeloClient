#ifndef __H_PUB_PLATFORM__
#define __H_PUB_PLATFORM__

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNUC__)
#define PLATFORM_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#endif

#if defined(_MSC_VER)
#define COMPILER_MSVC
#elif defined(__GNUC__)
#define COMPILER_GCC
#endif

#if defined(PLATFORM_WINDOWS)

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"ws2_32.lib")

#elif defined(PLATFORM_LINUX)

#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>

#endif

typedef int bool_t;
#define True 1
#define False 0

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#if defined(COMPILER_MSVC)

typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;
typedef INT8 int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;

#endif

// Standard ANSI include files
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>
#include <limits.h>

#ifdef __cplusplus

// STL header files
#include <string>
#include <fstream>
#include <strstream>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <exception>
#include <deque>
#include <exception>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>

#endif

typedef std::basic_string<unsigned char> blob_t;

#endif // __H_PUB_PLATFORM__
