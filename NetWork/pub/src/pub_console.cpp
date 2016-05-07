#include "pub.h"
#include "pub_console.h"

#if defined(PLATFORM_WINDOWS)
void PUB_console_set_info(PHANDLER_ROUTINE handler) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD buffSize = {108, 320};
	SMALL_RECT rc = {0, 0, buffSize.X - 1, 27};

	SetConsoleScreenBufferSize(hConsole, buffSize);
	SetConsoleWindowInfo(hConsole, true, &rc);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	SetConsoleCtrlHandler(handler, true);
}
#endif

#if defined(PLATFORM_LINUX)
bool_t PUB_console_set_daemon(PUB_PFN_DAEMON_QUIT cb) {
	bool ret = false;
	int ret_code = 0;
	int null_file = -1;
	rlimit res_limit;

	res_limit.rlim_cur = SHRT_MAX;
	res_limit.rlim_max = SHRT_MAX;

	ret_code = setrlimit(RLIMIT_NOFILE, &res_limit);
	PUB_PROCESS_ERROR(ret_code == 0);

	ret_code = getrlimit(RLIMIT_CORE, &res_limit);
	PUB_PROCESS_ERROR(ret_code == 0);
	
	res_limit.rlim_cur = res_limit.rlim_max;

	ret_code = setrlimit(RLIMIT_CORE, &res_limit);
	PUB_PROCESS_ERROR(ret_code == 0);

	signal(SIGINT, cb);
	signal(SIGQUIT, cb);
	signal(SIGTERM, cb);

	null_file = open("/dev/null", O_WRONLY);
	PUB_PROCESS_ERROR(null_file != -1);

	dup2(null_file, STDIN_FILENO);
	dup2(null_file, STDOUT_FILENO);
	dup2(null_file, STDERR_FILENO);

	ret = True;

Exit0:
	if (null_file != -1) {
		close(null_file);
		null_file = -1;
	}

	return ret;
}

#endif
