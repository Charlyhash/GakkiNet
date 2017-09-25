#ifndef __CONDITION_H__
#define __CONDITION_H__
/*
对条件变量的封装
*/
#include "NoCopyAble.h"
#include "Mutex.h"
#include <pthread.h>
#include <error.h>

namespace GakkiNet
{

	class Condition : public NoCopyAble
	{
	public:
		explicit Condition(MutexLock& mutex) : mutex_(mutex)
		{
			if (pthread_cond_init(&pcond_, NULL) != 0)
			{
				LOG(ERROR) << "pthread_cond_init";
				exit(-1);
			}
		}

		~Condition()
		{
			if (pthread_cond_destroy(&pcond_) != 0)
			{
				LOG(ERROR) << "pthread_cond_destroy";
				exit(-1);
			}
		}
		//等待信号
		void wait()
		{
			if (pthread_cond_wait(&pcond_, mutex_.getMutex()) != 0)
			{
				LOG(ERROR) << "pthread_cond_wait";
				exit(-1);
			}
		}
		//发送通知
		void notify()
		{
			pthread_cond_signal(&pcond_);
		}
		//发送广播通知
		void notifyAll()
		{
			pthread_cond_broadcast(&pcond_);
		}

		bool waitForSeconds(int seconds)
		{
			struct timespec abstime;
			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec += seconds;

			return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getMutex(), &abstime);
		}

	private:
		MutexLock& mutex_;
		pthread_cond_t pcond_;
	};
}

#endif