#include "Thread.h"
#include "CurrentThread.h"

#include <string>
#include <functional>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace GakkiNet;

void threadFunc()
{
	cout << "tid = " << CurrentThread::tid() << endl;
}

void threadFunc2(int x)
{
	cout << "tid = " << CurrentThread::tid() << ", x = " << x << endl;
}

class Foo
{
public:
	explicit Foo(double x)
		: x_(x)
	{
	}

	void memberFunc()
	{
		printf("tid=%d, Foo::x_=%f\n", CurrentThread::tid(), x_);
	}

	void memberFunc2(const std::string& text)
	{
		printf("tid=%d, Foo::x_=%f, text=%s\n", CurrentThread::tid(), x_, text.c_str());
	}

private:
	double x_;
};

int main()
{
	printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
	///�����߳�t1:ʹ�ú���
	Thread t1(threadFunc);
	t1.start();
	t1.join();
	///�����߳�t2:ʹ��bind�󶨲���
	Thread t2(bind(threadFunc2, 42),"thread for free function with argument");
	t2.start();
	t2.join();

	/// �����߳�t3:ʹ���࣬��ʹ��bind��
	Foo foo(87.53);
	Thread t3(bind(&Foo::memberFunc, &foo),
		"thread for member function without argument");
	t3.start();
	t3.join();

	///�����߳�t4:ʹ����ĺ�����ref()�����ã�
	Thread t4(bind(&Foo::memberFunc2, ref(foo), std::string("Gakki")));
	t4.start();
	t4.join();

	printf("number of created threads %d\n", Thread::numCreated());
}