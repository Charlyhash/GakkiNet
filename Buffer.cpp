#include "Buffer.h"
#include "SocketOperation.h"
#include <errno.h>
#include <sys/uio.h>
#include <fcntl.h>

const uint32_t GakkiNet::Buffer::InitSize = 1024;

//结合栈上空间读取IO的数据到缓存
int GakkiNet::Buffer::readFromIO(int fd, int& errorNo)
{
	// 节省一次ioctl系统调用（获取有多少可读数据）
	char extrabuf[65536]; //64K
	struct iovec vec[2];
	const size_t writable = writableBytes();
	// 第一块缓冲区
	vec[0].iov_base = &(*buffer_.begin()) + writeIndex_;
	vec[0].iov_len = writable;
	// 第二块缓冲区
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	const size_t n = SocketOperation::readv(fd, vec, 2);
	if (n < 0)
	{
		errorNo = errno;
	}
	else if ((uint32_t)n <= writable)	//第一块缓冲区足够容纳
	{
		writeIndex_ += n;
	}
	else// 当前缓冲区，不够容纳，因而数据被接收到了第二块缓冲区extrabuf，将其append至buffer
	{
		writeIndex_ = buffer_.size();
		append(extrabuf, n - writable);
	}

	return n;
}

void GakkiNet::Buffer::swap(Buffer & rhs)
{
	buffer_.swap(rhs.buffer_);
	std::swap(readIndex_, rhs.readIndex_);
	std::swap(writeIndex_, rhs.writeIndex_);
}
