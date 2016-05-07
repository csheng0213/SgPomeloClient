#include "pub.h"
#include "pub_random.h"

void PUB_srandom(uint32_t seed) {
#if defined(PLATFORM_LINUX)
	srandom((unsigned int)seed);
#elif defined(PLATFORM_WINDOWS)
	srand((unsigned int)seed);
#endif
}

uint32_t PUB_random() {
#if defined(PLATFORM_LINUX)
	return random();
#elif defined(PLATFORM_WINDOWS)
	return rand();
#endif
}

