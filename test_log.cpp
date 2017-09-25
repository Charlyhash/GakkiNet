#include "GakkiLog.h"
#include <pthread.h>

void* func(void *arg)
{
    LOG(INFO) << "INFO in func -1" ;
    LOG(INFO) << "INFO in func -2" ;

    return NULL;
}

void* func2(void* arg)
{
    LOG(INFO) << "INFO in func2 -1";
    LOG(INFO) << "INFO in func2 -2";

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    int ret = pthread_create(&tid1, NULL, func, NULL);
    if (ret != 0)
        LOG(ERROR) << "Create thread";
    ret = pthread_create(&tid2, NULL, func2, NULL);
    if (ret != 0)
        LOG(ERROR) << "Create thread";
    LOG(INFO) << "INFO" << " dd " << 333 << " 444";
    LOG(ERROR) << "ERROR";

    ret = pthread_join(tid1, NULL);
    if (ret != 0)
        LOG(ERROR) << "ERROR";
    ret = pthread_join(tid2, NULL);
    if (ret != 0)
        LOG(ERROR) << "ERROR";
    pthread_join(tid2, NULL);
    if (ret != 0)
        LOG(ERROR) << "ERROR";

    //core_dump
    //func();
    return 0;
}