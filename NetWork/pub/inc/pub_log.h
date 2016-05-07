#ifndef _PUB_LOG_H_
#define _PUB_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	enum LOG_TYPE {
		LT_INFO,
		LT_ERROR,
	};
	
	PUB_API bool_t PUB_log_file(const char* file_name, const char* format, ...);
	PUB_API bool_t PUB_log(enum LOG_TYPE type, const char* format, ...);
	PUB_API void PUB_log_set_dir(const char* dir);
	PUB_API void PUB_log_console_switch(bool_t enable);	
	PUB_API void PUB_log_set_file_name(enum LOG_TYPE type, const char* file_name);	
	
#ifdef __cplusplus
};
#endif

#endif /* _PUB_LOG_H_ */
