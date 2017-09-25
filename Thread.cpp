#include "Thread.h"
#include "GakkiLog.h"
#include "CurrentThread.h"
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

namespace GakkiNet
{

    namespace CurrentThread
    {
        __thread int t_cachedTid = 0;
        __thread const char* t_threadName = "unknown";
        const bool sameType = std::is_same<int, pid_t>::value;
        static_assert(sameType, "sameType is false:pid_t,int");
    }

    namespace detail
    {
        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid)); //得到tid
        }

        void afterFork()
        {
            GakkiNet::CurrentThread::t_cachedTid = 0;
            GakkiNet::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
        }

        class ThreadNameInitializer
        {
         public:
          ThreadNameInitializer()
          {
            GakkiNet::CurrentThread::t_threadName = "main";
            CurrentThread::tid();
            pthread_atfork(NULL, NULL, &afterFork);
          }
        };

        ThreadNameInitializer init; //全局的变量，会自动缓存主线的tid和名字
    }
}

using namespace GakkiNet;

//缓存tid
void CurrentThread::cacheTid()
{
    if (CurrentThread::t_cachedTid == 0)
    {
		CurrentThread::t_cachedTid = detail::gettid();
    }
}

//判断是不是主线程
bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
    : started_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n)
{
    numCreated_.increment();
}

Thread::~Thread() {}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    int ret = pthread_create(&pthreadId_, NULL, startThread, this);
    if (ret != 0)
        LOG(ERROR) << "pthread_create";
}

int Thread::join()
{
    assert(started_);
    return pthread_join(pthreadId_, NULL);
}

void* Thread::startThread(void* thread)
{
    Thread* th = static_cast<Thread*>(thread);
    th->runInThread();
	return NULL;
}
void Thread::runInThread()
{
    tid_ = CurrentThread::tid();
	CurrentThread::t_threadName = name_.empty() ? "GakkiThread" : name_.c_str();

	func_();
	CurrentThread::t_threadName = "finished";
}

