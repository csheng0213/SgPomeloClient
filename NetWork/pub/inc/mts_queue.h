#ifndef __MTSQUEUE_H__
#define __MTSQUEUE_H__

#include "lock.h"
#include <queue>

template< class _Ty, class _Container = std::deque<_Ty> >
class mts_queue
{
	typedef typename _Container::value_type value_type;
	typedef typename _Container::reference reference;
	typedef typename _Container::const_reference const_reference;
public:
	bool Empty() const {
		AutoLock<MutexLock> lock(const_cast<MutexLock &>(m_Lock));
		return queue.empty();
	}

	void Push(value_type&& _Val) {
		AutoLock<MutexLock> lock(m_Lock);
		queue.push(_Val);
	}

	void Push(const value_type& _Val) {
		AutoLock<MutexLock> lock(m_Lock);
		queue.push(_Val);
	}

	void Pop() {
		AutoLock<MutexLock> lock(m_Lock);
		queue.pop();
	}

	reference Front() {
		AutoLock<MutexLock> lock(m_Lock);
		return queue.front();
	}

	const_reference Front() const {
		AutoLock<MutexLock> lock(m_Lock);
		return queue.front();
	}

	reference Back() {
		AutoLock<MutexLock> lock(m_Lock);
		return queue.back();
	}

	const_reference Back() const {
		AutoLock<MutexLock> lock(m_Lock);
		return queue.back();
	}

private:
	std::queue<_Ty, _Container> queue;
	MutexLock m_Lock;
};

#endif