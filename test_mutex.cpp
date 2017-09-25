#include "Mutex.h"
#include "GakkiLog.h"

#include <vector>
#include <iostream>
#include <pthread.h>

using namespace GakkiNet;
using namespace std;

MutexLock g_mutex;
int g_val = 1;
const int kCount = 10*1000;

void* func(void *arg)
{
    while (true)
    {
        {
            MutexLockGuard lock(g_mutex);
            cout <<  (char*)arg << ":" <<g_val++ << endl;
        }
        if (g_val > kCount)
            break;
    }
    return NULL;
}


int main()
{
    pthread_t tid1, tid2;
    if (pthread_create(&tid1, NULL, func, (void *)"thread 1") != 0)
        LOG(ERROR) << "pthread_create";
    if (pthread_create(&tid2, NULL, func, (void *)"thread 2") != 0)
        LOG(ERROR) << "pthread_create";

    // while (true)
    // {
    //     MutexLockGuard lock(g_mutex);
    //     cout << g_val++ << endl;
    //     if (g_val > kCount)
    //         break;
    // }

    int ret = pthread_join(tid1, NULL);
    if (ret != 0)
    {
        LOG(INFO) << "pthread exit code :" << ret ;
        LOG(ERROR) << "pthread_join";
    }
    ret = pthread_join(tid2, NULL);
    if (ret != 0)
    {
        LOG(INFO) << "pthread exit code :" << ret ;
        LOG(ERROR) << "pthread_join";
    }

    return 0;
}