#include "EventLoop.h"
#include "Timer.h"
#include <memory>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "CurrentThread.h"

using namespace GakkiNet;
using namespace std;

int cnt = 0;
EventLoop* g_loop;

void PrintTid()
{
	printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	printf("now %s\n", Timer::getNowTimeDate().c_str());
}

int main()
{
	PrintTid();
	EventLoop loop;
	g_loop = &loop;
	g_loop->runInLoop(PrintTid);
    g_loop->runEveryInterval(PrintTid, 3);
	g_loop->run();
	
	return 0;
}