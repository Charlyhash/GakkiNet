#ifndef __TIMER_H__
#define __TIMER_H__

#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <functional>
#include <memory>
using namespace std;
/*
Timer是对事件类事件的封装，包含一个到期时间和处理函数
*/
namespace GakkiNet
{
	class Timer
	{
	public:
		Timer(uint32_t interval, const function<void()> & funcHandle);
		struct timeval getTimeOut();
		uint64_t getTimeOutMSecond();
		struct timespec getTimeInterval();
		void update();
		void setHandle(const function<void()> &);

		static uint64_t getNowTimeMSecond();
		static std::string getNowTimeDate();

		void timerHandle();
	private:
		uint32_t intervalMs; //定时时间
		struct timeval now; //当前时间
		function<void()> handle; //时间到后的处理函数
	};
}


#endif 
