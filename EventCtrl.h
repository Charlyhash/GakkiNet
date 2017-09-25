#ifndef __EVENT_CTRL_H__
#define __EVENT_CTRL_H__

#include <vector>
#include <map>
#include <memory>
#include "Epoll.h"
#include "Event.h"
/*
���¼��������࣬�����¼�ѭ���еȴ��¼����������ö�Ӧ�Ļص�����, ��������һ��eventloop,
��eventloopʹ�á����¼��Ĳ���ͬ�¸��µ�eventloop��eventloop��¼�����е��¼�����ͬʱ��
Ҫ���¶�Ӧ��epoll�ļ���
*/

namespace GakkiNet
{
	class EventLoop;
	class EventCtrl
	{
	public:
		EventCtrl(EventLoop* lo);
		~EventCtrl();
		//����¼�
		void addEvent(shared_ptr<Event> event);
		//ɾ���¼�
		void deleteEvent(shared_ptr<Event> event);
		//�޸��¼�
		void modifyEvent(shared_ptr<Event> event);
		//�޸��¼�
		void modifyEvent(int fd);
		//ɾ���¼�
		void deleteEvent(int fd);
		//�ȴ��¼�ѭ�������ö�Ӧ�Ļص�����
		void waitAndRunHandle(int timeMs);
	private:
		static const int activeEventLength; //����¼��б���
		std::map<int, weak_ptr<Event> > eventPool;//fd��event��ӳ��
		Epoll epoll; //�¼�ע�ᵽ���epoll
		EventLoop* loop; //������event loop
		std::vector<struct epoll_event> activeEvents; //��Ծ��ʱ��
	};
}

#endif 
