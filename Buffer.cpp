#include "Buffer.h"
#include "SocketOperation.h"
#include <errno.h>
#include <sys/uio.h>
#include <fcntl.h>

const uint32_t GakkiNet::Buffer::InitSize = 1024;

//���ջ�Ͽռ��ȡIO�����ݵ�����
int GakkiNet::Buffer::readFromIO(int fd, int& errorNo)
{
	// ��ʡһ��ioctlϵͳ���ã���ȡ�ж��ٿɶ����ݣ�
	char extrabuf[65536]; //64K
	struct iovec vec[2];
	const size_t writable = writableBytes();
	// ��һ�黺����
	vec[0].iov_base = &(*buffer_.begin()) + writeIndex_;
	vec[0].iov_len = writable;
	// �ڶ��黺����
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	const size_t n = SocketOperation::readv(fd, vec, 2);
	if (n < 0)
	{
		errorNo = errno;
	}
	else if ((uint32_t)n <= writable)	//��һ�黺�����㹻����
	{
		writeIndex_ += n;
	}
	else// ��ǰ���������������ɣ�������ݱ����յ��˵ڶ��黺����extrabuf������append��buffer
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
