#ifndef _PUB_CONSOLE_H_
#define _PUB_CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#if defined(PLATFORM_LINUX)
	typedef void (*PUB_PFN_DAEMON_QUIT) (int);
	bool_t PUB_console_set_daemon(PUB_PFN_DAEMON_QUIT cb);
#endif

#if defined(PLATFORM_WINDOWS)
	PUB_API void PUB_console_set_info(PHANDLER_ROUTINE handler);
#endif

#ifdef __cplusplus
};
#endif

#endif /* _PUB_CONSOLE_H_ */
