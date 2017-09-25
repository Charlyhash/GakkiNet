.PHONY:clean all

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

LINK = -lglog -lpthread
CPPCOMPILE = $(CXX) -c $(CXXFLAGS) $< -o $@
CPPLINK = $(CXX) $(CXXFLAGS) $^ -o $@
OBJS = EventLoop.o EventLoopThread.o EventCtrl.o Thread.o \
		Epoll.o Event.o TimerQueue.o Timer.o 
OBJS2 = TcpAccept.o TcpConnect.o TcpServer.o Socket.o SocketAddr.o SocketOperation.o EventLoopThreadPool.o Buffer.o
BIN = test_log test_mutex test_thread test_socket test_event_loop \
	test_event_loop_thread test_tcp_server test_tcp_server_threadpoll SudokuServer
all: $(BIN)

test_log:test_log.o 
	$(CPPLINK) $(LINK)
test_mutex:test_mutex.o 
	$(CPPLINK) $(LINK)
test_thread: test_thread.o Thread.o
	$(CPPLINK) $(LINK)
test_socket:test_main.o Socket.o SocketAddr.o SocketOperation.o
	$(CPPLINK) $(LINK)
# Timer.o EventLoop.o EventCtrl.o Epoll.o Event.o TimerQueue.o Thread.o 
test_event_loop:test_event_loop.o EventLoop.o Timer.o Thread.o  EventCtrl.o TimerQueue.o  Epoll.o Event.o 
	$(CPPLINK) $(LINK)

test_event_loop_thread:test_event_loop_thread.o $(OBJS)
	$(CPPLINK) $(LINK)

test_tcp_server: test_tcp_server.o $(OBJS) $(OBJS2)
	$(CPPLINK) $(LINK)
test_tcp_server_threadpoll: test_tcp_server_threadpoll.o $(OBJS) $(OBJS2)
	$(CPPLINK) $(LINK) 
SudokuServer:SudokuServer.o $(OBJS) $(OBJS2) suduku.o
	$(CPPLINK) $(LINK)

%.o : %.cpp
	$(CPPCOMPILE)

clean:
	rm -f $(BIN) *.o
