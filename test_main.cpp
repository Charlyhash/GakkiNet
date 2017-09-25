#include <iostream>

#include "Socket.h"
#include "SocketAddr.h"
#include "SocketOperation.h"
using namespace GakkiNet;
using namespace std;

int main()
{
	Socket socket(-1);
	cout << socket.getFd() << endl;
}