#include "EventLoop.h"
#include "EventLoopThread.h"
#include <stdio.h>
#include <unistd.h>

using namespace GakkiNet;

void runInThread()
{
	printf("runInThread(): pid = %d, tid = %d\n",
		getpid(), CurrentThread::tid());
}

int main()
{
	runInThread();

	EventLoopThread loopThread;
	EventLoop* loop = loopThread.startLoop();
	sleep(1);
	loop->runInLoop(runInThread);

	return 0;
}

