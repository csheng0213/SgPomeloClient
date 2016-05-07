#ifndef __PUB_DIR_H__
#define __PUB_DIR_H__

#if defined(PLATFORM_LINUX)
#	include <sys/stat.h>
#	include <sys/types.h>
#elif defined(PLATFORM_IOS)
#	include <sys/stat.h>
#	include <sys/types.h>
#elif defined(PLATFORM_WINDOWS)
#	include <direct.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
#if defined(PLATFORM_LINUX)
#	define MKDIR(path) ::mkdir((path), 0777)
#elif defined(PLATFORM_IOS)
#	define MKDIR(path) ::mkdir((path), 0777)
#elif defined(PLATFORM_WINDOWS)
#	define MKDIR(path) ::_mkdir((path))
#endif

#define RMFILE(file) ::remove((file))
#define RMDIR(path) ::rmdir((path))

#ifdef __cplusplus
};
#endif

#endif