#ifndef __LOCK_H__
#define __LOCK_H__

#if defined(PLATFORM_WINDOWS)
#	include <Windows.h>
#endif
#include <pthread.h>

#if defined(PLATFORM_WINDOWS)
class CriticalLock {
public:
	void Lock() { EnterCriticalSection(&cs); }
	void Unlock() { LeaveCriticalSection(&cs); }

public:
	CriticalLock() { InitializeCriticalSection(&cs); }
	~CriticalLock() { DeleteCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
};
#endif

class MutexLock {
public:
	void Lock() { pthread_mutex_lock(&mutex); }
	void Unlock() { pthread_mutex_unlock(&mutex); }

public:
	MutexLock() { pthread_mutex_init(&mutex, NULL); }
	~MutexLock() { pthread_mutex_destroy(&mutex); }

private:
	pthread_mutex_t mutex;
};

template<class T>
class AutoLock {
public:
	AutoLock(T &lock, bool bLock = true) : lock(lock), bLock(bLock) { if (bLock) lock.Lock(); }
	~AutoLock() { if (bLock) lock.Unlock(); }

private:
	T &lock;
	bool bLock;
};

#endif