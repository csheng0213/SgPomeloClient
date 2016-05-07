#include "pub.h"

uint64_t PUB_get_cur_tick() {
#if defined(PLATFORM_WINDOWS)
	FILETIME ft;
	ULARGE_INTEGER li;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart  = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	return li.QuadPart / 10000;	
#elif defined(PLATFORM_LINUX)
	struct timeval tv;
	gettimeofday (&tv, NULL);
	return (uint64_t) ((uint64_t) tv.tv_sec * 1000 + (uint64_t) tv.tv_usec / 1000);
#endif
}

void PUB_sleep(uint64_t ms) {
#if defined(PLATFORM_WINDOWS)
	Sleep((DWORD)ms);
#elif defined(PLATFORM_LINUX)
	usleep(ms * 1000);
#endif
}

