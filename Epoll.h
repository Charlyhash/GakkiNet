#ifndef __EPOLL_H__
#define __EPOLL_H__
/*
��linux epoll�ķ�װ,����һ��epollFd.ÿ��epoll������һ��event
*/
#include "NoCopyAble.h"
#include <sys/epoll.h>
#include "Event.h"

namespace GakkiNet
{
	class Epoll :NoCopyAble
	{
	public:
		//���캯��
		Epoll();
		//��������
		~Epoll();
		//����¼�
		bool addEvent(Event* event);
		//�Ƴ�
		bool removeEvent(Event* event);
		//�޸�
		bool modifyEvent(Event* evnet);
		//�Ƴ�
		bool removeEvent(int fd);
		//�ȴ�
		int waitEvent(struct epoll_event* eventList, int eventSize, int timeMs);

	private:
		//��fd���в���
		int epollCtrl(int op, int fd, int events);
		int epollFd; //epoll��fd,���캯��ʱʹ��epoll_create����
	};

}

#endif
