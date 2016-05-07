#include "pub.h"

bool_t PUB_is_abs_path(const char* path) {
#if defined(PLATFORM_WINDOWS)
	if (isalpha((int)(path[0])) && path[1] == ':')
#elif defined(PLATFORM_LINUX)
	if (path[0] == '/')
#endif
		return True;
	return False;
}

std::string PUB_cat_path(const std::string& base, const std::string& name) {
	if (base.empty() || PUB_is_abs_path(name.c_str()))
		return name;
	else if (base.size() > 0 && base[base.size() - 1]!='\\' && 
		base[base.size() - 1] != '/')
		return base + '/' + name;
	else
		return base + name;
}

