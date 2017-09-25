#ifndef __MUTEX_H__
#define __MUTEX_H__
/*
对mutex的封装
*/

#include "NoCopyAble.h"
#include "GakkiLog.h"
#include <pthread.h>

namespace GakkiNet
{
    
	//mutex封装，作为内部使用
	class MutexLock : public NoCopyAble
	{
	public:
		MutexLock()
		{
			int ret = pthread_mutex_init(&mutex_, NULL);
			if(ret != 0)
			{
				LOG(ERROR) << "pthread_mutex_init";
				exit(-1);
			}
		}
		~MutexLock()
		{
			int ret = pthread_mutex_destroy(&mutex_);
			if(ret != 0)
			{
				LOG(ERROR) << "pthread_mutex_init";
				exit(-1);
			}
		}

		void lock()
		{
			int ret = pthread_mutex_lock(&mutex_);
			if(ret != 0)
			{
				LOG(ERROR) << "pthread_mutex_lock";
				exit(-1);
			}
		}

		void unLock()
		{
			int ret = pthread_mutex_unlock(&mutex_);
			if(ret != 0)
			{
				LOG(ERROR) << "pthread_mutex_unlock";
				exit(-1);
			}
		}

		pthread_mutex_t* getMutex()
		{
			return &mutex_;
		}

	private:
		pthread_mutex_t mutex_;
	};

	//对一段代码上锁，无需解锁,析构时自动解锁
	class MutexLockGuard : public NoCopyAble
	{
	public:
		explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
		{
			mutex_.lock();
		}
		
		~MutexLockGuard()
		{
			mutex_.unLock();
		}
	private:
		MutexLock& mutex_;
	};

}

#endif