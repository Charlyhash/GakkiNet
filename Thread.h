#ifndef __THREAD_H__
#define __THREAD_H__
/*
thread类的封装，只需要传递一个函数，调用start()便可以运行
*/
#include "AtomicInt.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <functional>
#include <string>
using namespace std;

namespace GakkiNet
{
	class Thread
	{
	public:
		typedef std::function<void ()> ThreadFunc;
		explicit Thread(const ThreadFunc&, const string& name = string());
		~Thread();
		void start();
		int join();
		bool started() const { return started_;}
		pid_t tid() const { return tid_; }
		const string& name() const { return name_; }
		static int numCreated() { return numCreated_.get(); }


	private:
		static void* startThread(void* thread); //开启线程调用的函数
		void runInThread(); //供startThread调用,调用func_

		bool started_;
		pthread_t pthreadId_;
		pid_t tid_;
		ThreadFunc func_;
		string name_;
		static AtomicInt32 numCreated_; //开启的线程数目
	};

}

#endif