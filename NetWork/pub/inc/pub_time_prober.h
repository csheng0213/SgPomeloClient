#ifndef __H_PUB_TIME_PROBER__
#define __H_PUB_TIME_PROBER__

#ifdef __cplusplus

// profiler
const uint32_t PPF_MAX_TIME = 1;
const uint32_t PPF_CLEAR = 1 << 1;

namespace pub {
	struct Profiler {
		const Profiler* parent;
		const char* constName;
		uint64_t seg;
		Profiler( const char* cn );
		~Profiler();
	};
	
	class HistoryProfiler {
	public:
		HistoryProfiler(const char* desc);
		~HistoryProfiler();
	private:
		const char* _desc;
		uint64_t _startTick;
	};
}

#if PROFILE_ENABLE

#define _NUM2STR(n) #n
#define NUM2STR(n) _NUM2STR(n)
#define PROFILER pub::Profiler __profiler( __FILE__ "(" NUM2STR(__LINE__) ") : ");
#define PROFILERX(desc) pub::Profiler __profiler( __FILE__ "(" NUM2STR(__LINE__) ") : [" desc "]" );
#define HIS_PROFILER pub::HistoryProfiler __his_profiler( __FILE__ "(" NUM2STR(__LINE__) "): " );

#else

#define PROFILER
#define PROFILERX( desc )
#define HIS_PROFILER

#endif

void PUB_prof_print(uint32_t flags);
void PUB_prof_enable(bool_t b);
void PUB_prof_enable_tree(bool_t b);
void PUB_prof_enable_log_to_file(bool_t b);

void PUB_hisprof_enable(bool_t enable);
void PUB_hisprof_log_to_file(const char* fileName);
void PUB_hisprof_set_limit(size_t maxFrame, size_t maxRecPerFrame);
void PUB_hisprof_begin_frame();
void PUB_hisprof_end_frame();
void PUB_hisprof_enable_log_every_frame(bool_t enable);

#endif

#endif // __H_PUB_TIME_PROBER__
