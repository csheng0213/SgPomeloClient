#include "pub.h"

namespace pub {
	
	class ProfileManager {
		
		struct Node {
			Node() { memset( this, 0, sizeof *this ); }
			uint32_t times, max_times, prev_times;
			uint64_t ticks, max_ticks, prev_ticks;
			void UpdateFrame() {
				prev_ticks = ticks;
				prev_times = times;
				if ( ticks > max_ticks ) max_ticks = ticks;
				if ( times > max_times ) max_times = times;
				ticks = 0;
				times = 0;
			}
		};
		
		struct Path {
			struct PathIterator {
				typedef std::bidirectional_iterator_tag iterator_category;
				typedef Profiler const* value_type;
				typedef ptrdiff_t difference_type;
				typedef const value_type* pointer;
				typedef const value_type& reference;

				Profiler const *it;
				PathIterator( Profiler const *p ) : it( p ) {}
				operator const char* () const { return it ? it->constName : NULL; }
				void operator ++ () { if ( it ) it = it->parent; }
				const char* operator * () const { return it ? it->constName : NULL; }
			};
			std::vector< const char* > fullpath;
			const Profiler *nodepath;
			Path( const Profiler *p ) : nodepath( p ) {}
			Path( const Path &o ) : nodepath( NULL ) {
				Profiler const *iter = o.nodepath;
				if ( iter == NULL ) {
					fullpath = o.fullpath;
				} else {
					while ( iter ) {
						fullpath.push_back( iter->constName );
						iter = iter->parent;
					}
				}
			}

			bool operator < ( const Path &o ) const {
				if ( nodepath )
					return std::lexicographical_compare( PathIterator( nodepath ), 
														 PathIterator( NULL ), 
														 o.fullpath.begin(), 
														 o.fullpath.end() );
				else if ( o.nodepath )
					return std::lexicographical_compare( fullpath.begin(), 
														 fullpath.end(), 
														 PathIterator( o.nodepath ), 
														 PathIterator( NULL ) );
				return fullpath < o.fullpath;
			}
		};

		std::map< Path, Node > paths;

		std::map< const char*, Node > nodes;

		bool_t isTree;
		bool_t isEnable;
		bool_t isLogToFile;

		std::ofstream logFile;

	public:

		static ProfileManager& getSingleton() {
			static ProfileManager s;
			return s;
		}

		ProfileManager() {
			isTree = false;
			isEnable = false;
			isLogToFile = true;
		}

		void Enable(bool_t enable) { isEnable = enable; }
		bool_t IsEnable() { return isEnable; }
		void EnableTreeLog(bool_t tree) { isTree = tree; }
		void EnableLogToFile(bool_t tofile) { isLogToFile = tofile; }

		void Log(const char* txt)
		{
			if (isLogToFile)
				logFile << txt;
			else
				std::cout << txt;
		}

		void Update( const Profiler *p, uint64_t ticks ) {
			if (isTree) {
				Node *n = &paths[ Path( p ) ];
				n->ticks += ticks;
				n->times ++;
				if ( p->parent == NULL ) {
					for ( std::map< Path, Node >::iterator it = paths.begin(); it != paths.end(); it++ )
						it->second.UpdateFrame();
				}
			}
			else {
				Node *n = &nodes[ p->constName ];
				n->ticks += ticks;
				n->times ++;
				if ( p->parent == NULL ) {
					// 已经到了栈顶，更新最大值
					for ( std::map< const char*, Node >::iterator it = nodes.begin(); it != nodes.end(); it++ )
						it->second.UpdateFrame();
				}
			}
		}

		void Print( uint32_t flags ) {
			if (isLogToFile)
				logFile.open("profile_output.txt", std::ios::out);

			if (isTree) {
				SortNode root;
				for ( std::map< Path, Node >::iterator it = paths.begin(); it != paths.end(); it++ )
					root.Insert( it->first, it->second );
				root.Print( flags );
			}
			else {
				static char str[1024];
				for ( std::map< const char*, Node >::iterator it = nodes.begin(); it != nodes.end(); it++ ) {
					if ( flags & PPF_MAX_TIME )
						sprintf_safe( str, "%s\t[\t%llu\t%d\t]\n", it->first, it->second.max_ticks, it->second.max_times );
					else
						sprintf_safe( str, "%s\t[\t%llu\t%d\t]\n", it->first, it->second.prev_ticks, it->second.prev_times );
					Log(str);
					if ( flags & PPF_CLEAR ) {
						it->second.max_ticks = 0;
						it->second.max_times = 0;
					}
				}
			}

			logFile.close();
		}

		struct SortNode {
			Node *nodePtr;
			std::map< const char*, SortNode > childs;

			SortNode() : nodePtr( NULL ) {}

			void Insert( const Path &path, Node &node ) {
				SortNode *iter = this;
				for ( uint32_t i = (uint32_t)path.fullpath.size(); i != 0; i-- )
					iter = &iter->childs[ path.fullpath[i-1] ];
				if ( iter->nodePtr != NULL )
					throw 0;
				iter->nodePtr = &node;
			}

			void Print( uint32_t flags, uint32_t level = 0 ) {
				static char str[1024];
				memset( str, '\t', level );
				for ( std::map< const char*, SortNode >::iterator it = childs.begin(); 
					  it != childs.end(); it++ ) {
					if ( flags & PPF_MAX_TIME ) {
						sprintf_unsafe( str + level, _countof(str) - level, "%s [\t%llu\t%d\t]\n", 
										it->first, 
										it->second.nodePtr->max_ticks, 
										it->second.nodePtr->max_times );
					}
					else {
						sprintf_unsafe( str + level, _countof(str) - level, "%s [\t%llu\t%d\t]\n", 
										it->first, 
										it->second.nodePtr->prev_ticks, 
										it->second.nodePtr->prev_times );
					}

					ProfileManager::getSingleton().Log( str );

					if ( flags & PPF_CLEAR ) {
						it->second.nodePtr->max_ticks = 0;
						it->second.nodePtr->max_times = 0;
					}

					it->second.Print( flags, level+1 );
				}
			}
		};
	};

	const Profiler* s_current = NULL;

	Profiler::Profiler( const char* cn ) : constName( cn ) { 
		if (!ProfileManager::getSingleton().IsEnable())
			return;

		parent = s_current;
		seg = PUB_get_cur_tick();
		s_current = this;
	}

	Profiler::~Profiler() { 
		if (!ProfileManager::getSingleton().IsEnable())
			return;

		PUB_ASSERT(s_current == this);
		uint64_t now = PUB_get_cur_tick();
		ProfileManager::getSingleton().Update(s_current, now - seg);
		s_current = parent;	
	}

	//----------------------------------------------------------------
	
	class HistoryProfilerManager
	{
	public:
		HistoryProfilerManager() {
			_maxFrame = 10;
			_maxRecPerFrame = 10;
			_enable = False;
			_logEveryFrame = False;
		}

		static HistoryProfilerManager& Singleton() {
			static HistoryProfilerManager s;
			return s;
		}

		void AddHistoryProfiler(const char* desc, uint64_t consumeTick) {
			dequeTick_t& dequeTick = _currentFrame[desc];
			dequeTick.push_back(consumeTick);
			while (dequeTick.size() > _maxRecPerFrame)
				dequeTick.pop_front();
		}

		void LogToFile(const char* fileName) {
			std::ofstream logFile;
			logFile.open(fileName, std::ios::out);
			if (logFile.fail())
				return;
			
			int frame = 0;
			for (dequeFrame_t::iterator itFrame = _dequeFrameRecord.begin(); 
				 itFrame != _dequeFrameRecord.end(); ++itFrame) {
				struct tm* local = localtime(&itFrame->first);
				logFile << " Frame: " << frame << "[" 
						<< local->tm_hour << "-" << local->tm_min << "-" << local->tm_sec 
						<< "]" << std::endl;

				for (mapFameRecord_t::iterator itRecord = itFrame->second.begin(); 
					 itRecord != itFrame->second.end(); ++itRecord) {
					logFile << itRecord->first << ": ";
					for (dequeTick_t::iterator itTick = itRecord->second.begin(); 
						 itTick != itRecord->second.end(); ++itTick) {
						logFile << *itTick << ", ";
					}
					logFile << std::endl;
				}
				++frame;
			}
		}

		void SetLimit(size_t maxFrame, size_t maxRecPerFrame) {
			_maxFrame = maxFrame;
			_maxRecPerFrame = maxRecPerFrame;
		}

		void BeginFrame() {
			if (!IsEnable())
				return;

			_currentFrame.clear();
		}

		void EndFrame() {
			if (!IsEnable())
				return;

			if (_currentFrame.size() == 0)
				return;

			time_t t = time(NULL);
			_dequeFrameRecord.push_back(std::pair<time_t, mapFameRecord_t>(t, _currentFrame));

			if (_logEveryFrame) {
				for (mapFameRecord_t::iterator itRecord = _currentFrame.begin(); itRecord != _currentFrame.end(); ++itRecord) {
					std::cout << itRecord->first << ": ";
					for (dequeTick_t::iterator itTick = itRecord->second.begin(); itTick != itRecord->second.end(); ++itTick) {
						std::cout << *itTick << ", ";
					}
					std::cout << std::endl;
				}
			}

			while (_dequeFrameRecord.size() > _maxFrame)
				_dequeFrameRecord.pop_front();
		}

		void Enable(bool_t enable) { _enable = enable; }
		bool_t IsEnable() { return _enable; }
		void EnableLogEveryFrame(bool_t logEveryFrame) { _logEveryFrame = logEveryFrame; }

	private:
		typedef std::deque<uint64_t> dequeTick_t;
		typedef std::map<const char*, dequeTick_t> mapFameRecord_t;
		typedef std::deque< std::pair<time_t, mapFameRecord_t> > dequeFrame_t;

		dequeFrame_t _dequeFrameRecord;
		mapFameRecord_t _currentFrame;

		size_t _maxFrame;
		size_t _maxRecPerFrame;

		bool_t _enable;
		bool_t _logEveryFrame;
	};

	HistoryProfiler::HistoryProfiler(const char* desc) : _desc(desc) {
		if (!HistoryProfilerManager::Singleton().IsEnable())
			return;

		_startTick = PUB_get_cur_tick();
	}

	HistoryProfiler::~HistoryProfiler() {
		if (!HistoryProfilerManager::Singleton().IsEnable())
			return;

		uint64_t consumeTick = PUB_get_cur_tick() - _startTick;
		HistoryProfilerManager::Singleton().AddHistoryProfiler(_desc, consumeTick);
	}

}

void PUB_prof_print( uint32_t flags ) {
	pub::ProfileManager::getSingleton().Print( flags );
}

void PUB_prof_enable(bool_t b ) {
	pub::ProfileManager::getSingleton().Enable( b );
}

void PUB_prof_enable_tree( bool_t b ) {
	pub::ProfileManager::getSingleton().EnableTreeLog( b );
}

void PUB_prof_enable_log_to_file( bool_t b ) {
	pub::ProfileManager::getSingleton().EnableLogToFile( b );
}

void PUB_hisprof_enable(bool_t enable) {
	pub::HistoryProfilerManager::Singleton().Enable(enable);
}

void PUB_hisprof_log_to_file(const char* fileName) {
	pub::HistoryProfilerManager::Singleton().LogToFile(fileName);
}

void PUB_hisprof_set_limit(size_t maxFrame, size_t maxRecPerFrame) {
	pub::HistoryProfilerManager::Singleton().SetLimit(maxFrame, maxRecPerFrame);
}

void PUB_hisprof_begin_frame() {
	pub::HistoryProfilerManager::Singleton().BeginFrame();
}

void PUB_hisprof_end_frame() {
	pub::HistoryProfilerManager::Singleton().EndFrame();
}

void PUB_hisprof_enable_log_every_frame(bool_t enable) {
	pub::HistoryProfilerManager::Singleton().EnableLogEveryFrame(enable);
}
