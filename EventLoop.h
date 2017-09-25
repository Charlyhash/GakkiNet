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
�¼�ѭ����ÿ���¼�ѭ����Ӧ��һ���̣߳�����һ��EventCtrl��Ա����run�е��øó�Ա�ȴ��¼�
��ִ�ж�Ӧ�Ļص�����
*/
namespace GakkiNet 
{
	class EventLoop : public NoCopyAble
	{
	public:
		//���캯������Ҫ��ʼ��eventCtrl
		EventLoop();
		~EventLoop();
		//���¼�����
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
		static const int PollTimeMs; //poll�ȴ����¼�
		MutexLock mutex;
		shared_ptr<EventCtrl> eventCtrl; //������evnetCtrl
		pid_t threadId; //��ǰ�߳�id
		shared_ptr<TimerQueue> timerQueue; //������timer queue

		std::vector<function<void()>> functions; //�����̵߳��ú�����functions��
		void addFunInLoop(function<void()> func); //��ӵ�functions
		bool inThisThread(); //�ж��ǲ��Ǳ��̵߳���
		void runAllFunctionInLoop(); //�����̵߳��õĻص�ͳһ�������������
		
		int wakeupFd;//
		unique_ptr<Event> wakeupEvent;
		void wakeup(); //����
		void handleRead();//wake up

		bool callingFunctions;
	};
}

#endif // 
