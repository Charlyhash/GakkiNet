#ifndef __CURRENT_THREAD_H__
#define __CURRENT_THREAD_H__

/*
当前线程的缓存,包括tid和线程名
*/

namespace GakkiNet
{
    namespace CurrentThread
    {
        extern __thread int t_cachedTid; //缓存的tid
        extern __thread const char* t_threadName; //缓存的线程名
        void cacheTid(); //缓存tid

        inline int tid() //得到tid
        {
            if (t_cachedTid == 0)
            {
              cacheTid();
            }
            return t_cachedTid;
        }

        inline const char* name() //得到线程名
        {
            return t_threadName;
        }

        bool isMainThread(); //判断是不是主线程
    }
}

#endif