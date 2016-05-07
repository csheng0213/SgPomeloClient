#ifndef __H_PUB_ERROR__
#define __H_PUB_ERROR__

#define PUB_ASSERT(n) assert(n)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(n) \
	do { \
		if (n) { \
			(n)->Release(); \
			(n) = NULL; \
		} \
	} while (0)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(n) \
	do { \
		if (n) { \
			delete (n); \
			(n) = NULL; \
		} \
	} while (0)
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(n) \
	do { \
		if (n) { \
			delete [](n); \
			(n) = NULL; \
		} \
	} while (0)
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(n) \
	do { \
		if (n) { \
			free(n); \
			(n) = NULL; \
		} \
	} while (0)
#endif

#ifndef PUB_PROCESS_ERROR
#define PUB_PROCESS_ERROR(n) \
	do { \
		if ((n) < 0) { \
			PUB_log(LT_ERROR, "file: [%s] line: [%d] ecode: [%d]", __FILE__, __LINE__, (n)); \
			goto Exit0; \
		} \
	} while (0)
#endif

#ifndef PUB_PROCESS_ERROR_NOLOG
#define PUB_PROCESS_ERROR_NOLOG(n) \
	do { \
		if ((n) < 0) { \
			goto Exit0; \
		} \
	} while (0)
#endif

#ifndef PUB_PROCESS_ERROR_NOLOG_EXIT
#define PUB_PROCESS_ERROR_NOLOG_EXIT(n, e) \
	do { \
		if ((n) < 0) { \
			goto e; \
		} \
	} while (0)
#endif

#ifndef PUB_CHECK_CONDITION
#define PUB_CHECK_CONDITION(n) \
	do { \
		if (!(n)) { \
			PUB_log(LT_ERROR, "file: [%s] line: [%d]", __FILE__, __LINE__); \
			goto Exit0; \
		} \
	} while (0)
#endif

#ifndef PUB_CHECK_CONDITION_NOLOG
#define PUB_CHECK_CONDITION_NOLOG(n) \
	do { \
		if (!(n)) { \
			goto Exit0; \
		} \
	} while (0)
#endif

#ifndef PUB_CHECK_CONDITION_NOLOG_EXIT
#define PUB_CHECK_CONDITION_NOLOG_EXIT(n, e) \
	do { \
		if (!(n)) { \
			goto e; \
		} \
	} while (0)
#endif

#endif // __H_PUB_ERROR__
