#ifndef AGILNET_NET_IOEVENT
#define AGILNET_NET_IOEVENT
/*
对事件的封装，包含一个fd，每个事件都属于一个event loop。
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
		//得到fd
		int getFd(); 
		//得到events
		uint32_t getEvents();
		//允许读，写，错误的处理
		void enableReading(bool isEnable);
		void enableWriting(bool isEnable);
		void enableErrorEvent(bool isEnable);
		//全部静止
		void disableAll();
		//判断正在读
		bool isWriting();
		//判断正在写
		bool isReading();
		//移除
		void removeFromLoop();

		void handle(uint32_t revents);
		//设置读，写，出错，关闭的回调处理函数
		void setReadFunc(const EventCallBack& cb) { readHandle = cb; }
		void setWriteFunc(const EventCallBack& cb) { writeHandle = cb; }
		void setErrorFunc(const EventCallBack& cb) { errorHandle = cb; }
		void setCloseFunc(const EventCallBack& cb) { closeHandle = cb; }
	private:
		EventLoop* eventLoop; //event所属的eventloop
		void update(); 
		int eventFd; //event的fd，每个event负责一个fd
		uint32_t events;
		EventCallBack readHandle;
		EventCallBack writeHandle;
		EventCallBack errorHandle;
		EventCallBack closeHandle;
	};

}

#endif 
