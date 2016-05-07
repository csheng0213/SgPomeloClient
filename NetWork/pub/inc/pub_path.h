#ifndef _PUB_PATH_H_
#define _PUB_PATH_H_

#ifdef __cplusplus
extern "C" {
#endif
	
	bool_t PUB_is_abs_path(const char* path);
	
#ifdef __cplusplus
};
#endif

#ifdef __cplusplus

	std::string PUB_cat_path(const std::string& base, const std::string& name);
	
#endif // __cplusplus

#endif /* _PUB_PATH_H_ */
