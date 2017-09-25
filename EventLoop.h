#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <memory>
#include <vector>
#include <functional>
#include "Thread.h"
#include "Mutex.h"
#include "EventCtrl.h"
#include "CurrentThread.h"
#include "TimerQueue.h"
#include "NoCopyAble.h"
#include "GakkiLog.h"



using namespace std;
/*
事件循环，每个事件循环对应于一个线程，包含一个EventCtrl成员，在run中调用该成员等待事件
并执行对应的回调函数
*/
namespace GakkiNet 
{
	class EventLoop : public NoCopyAble
	{
	public:
		//构造函数，需要初始化eventCtrl
		EventLoop();
		~EventLoop();
		//对事件操作
		void addEvent(shared_ptr<Event> event);
		void removeEvent(shared_ptr<Event> event);
		void modifyEvent(shared_ptr<Event>  event);
		void modifyEvent(int fd);
		void removeEvent(int fd);
		void run();
		void runInLoop(function<void()> func);

		void runOniceAfter(function<void()> callback, uint32_t interval);
		void runEveryInterval(function<void()> callback, uint32_t interval);

		//void wakeup()

	private:
		static const int PollTimeMs; //poll等待的事件
		MutexLock mutex;
		shared_ptr<EventCtrl> eventCtrl; //包含的evnetCtrl
		pid_t threadId; //当前线程id
		shared_ptr<TimerQueue> timerQueue; //包含的timer queue

		std::vector<function<void()>> functions; //其他线程调用后都塞到functions中
		void addFunInLoop(function<void()> func); //添加到functions
		bool inThisThread(); //判断是不是本线程调用
		void runAllFunctionInLoop(); //其他线程调用的回调统一在这个函数处理
		
		int wakeupFd;//
		unique_ptr<Event> wakeupEvent;
		void wakeup(); //唤醒
		void handleRead();//wake up

		bool callingFunctions;
	};
}

#endif // 
