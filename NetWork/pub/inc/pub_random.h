#ifndef _PUB_RANDOM_H_
#define _PUB_RANDOM_H_

#ifdef __cplusplus
extern "C" {
#endif

	PUB_API void PUB_srandom(uint32_t seed);
	PUB_API uint32_t PUB_random();

#ifdef __cplusplus
}
#endif

#endif /* _PUB_RANDOM_H_ */
