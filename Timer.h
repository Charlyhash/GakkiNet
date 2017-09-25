#ifndef __TIMER_H__
#define __TIMER_H__

#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <functional>
#include <memory>
using namespace std;
/*
Timer�Ƕ��¼����¼��ķ�װ������һ������ʱ��ʹ�����
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
		uint32_t intervalMs; //��ʱʱ��
		struct timeval now; //��ǰʱ��
		function<void()> handle; //ʱ�䵽��Ĵ�����
	};
}


#endif 
