#ifndef __H_LIBPUB__
#define __H_LIBPUB__

#if defined(PUB_EXPORTS)
#	define PUB_API __declspec(dllexport)
#elif defined(PUB_IMPORT)
#	define PUB_API __declspec(dllimport)
#else
#	define PUB_API
#endif

#include "pub_platform.h"
#include "pub_safe_mem_op.h"
#include "pub_error.h"
#include "pub_time_prober.h"
#include "pub_time.h"
#include "pub_path.h"
#include "pub_log.h"
#include "pub_console.h"
#include "pub_ini.h"
#include "pub_random.h"
#include "pub_dir.h"
#include "xml.h"
#include "lzw.h"
#include "md5.h"

//#ifdef PLATFORM_WINDOWS
//#	include "stdapp.h"
//#endif

#endif // __H_LIBPUB__
