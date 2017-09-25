#ifndef AGILNET_NET_IOEVENT
#define AGILNET_NET_IOEVENT
/*
���¼��ķ�װ������һ��fd��ÿ���¼�������һ��event loop��
*/
#include <functional>
#include "NoCopyAble.h"
using namespace std;

namespace GakkiNet
{
	class EventLoop;

	class Event : public NoCopyAble
	{
	public:
		typedef function<void()> EventCallBack; 
		static const int noneEventFlag; 
		static const int readEventFlag; 
		static const int writeEventFlag;
		static const int errorEventFlag;

		explicit Event(EventLoop* loop, int fd);
		~Event();
		//�õ�fd
		int getFd(); 
		//�õ�events
		uint32_t getEvents();
		//�������д������Ĵ���
		void enableReading(bool isEnable);
		void enableWriting(bool isEnable);
		void enableErrorEvent(bool isEnable);
		//ȫ����ֹ
		void disableAll();
		//�ж����ڶ�
		bool isWriting();
		//�ж�����д
		bool isReading();
		//�Ƴ�
		void removeFromLoop();

		void handle(uint32_t revents);
		//���ö���д�������رյĻص�������
		void setReadFunc(const EventCallBack& cb) { readHandle = cb; }
		void setWriteFunc(const EventCallBack& cb) { writeHandle = cb; }
		void setErrorFunc(const EventCallBack& cb) { errorHandle = cb; }
		void setCloseFunc(const EventCallBack& cb) { closeHandle = cb; }
	private:
		EventLoop* eventLoop; //event������eventloop
		void update(); 
		int eventFd; //event��fd��ÿ��event����һ��fd
		uint32_t events;
		EventCallBack readHandle;
		EventCallBack writeHandle;
		EventCallBack errorHandle;
		EventCallBack closeHandle;
	};

}

#endif 
