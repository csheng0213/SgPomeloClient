#ifndef _PUB_TIME_H_
#define _PUB_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	PUB_API uint64_t PUB_get_cur_tick();
	PUB_API void PUB_sleep(uint64_t ms);
	
#ifdef __cplusplus
};
#endif

#endif /* _PUB_TIME_H_ */
