#ifndef __BUFFER_H__
#define __BUFFER_H__
/*
Buffer类：对网络的缓冲区的封装。不是线程安全的。
|------------------------------------|
|abcdefghijgk                        |
|------------|-----------------------|
readIndex    writeIndex
}
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include "GakkiLog.h"

#include "Buffer.h"

namespace GakkiNet
{
	using namespace std;
	class Buffer
	{
	public:
		static const uint32_t InitSize; //默认大小
		//构造函数
		explicit Buffer(uint32_t size = InitSize):readIndex_(0), writeIndex_(0), buffer_(size) {}
		//析构函数
		~Buffer() {}
		int readFromIO(int fd, int& errorNo);
		//可写的大小
		uint32_t writableBytes()
		{
			return buffer_.size() - writeIndex_; 
		}
		//可读的大小
		uint32_t readableBytes() const
		{
			return writeIndex_ - readIndex_;
		}
		//判断是否可读-可读的字节大于0
		bool isReadable()
		{
			return readableBytes() > 0 ? true : false;
		}
		//判断是否为空
		bool isEmpty()
		{
			return !isReadable();
		}
		//添加一个char*到缓冲区
		void append(const char* data, size_t len)
		{
			ensureWriteableBytes(len);
			copy(data, data + len, buffer_.begin() + writeIndex_);
			writeIndex_ += len;
		}
		//添加string到缓冲区
		void append(const string& str)
		{
			const char* data = &(*str.begin());
			append(data, str.size());
		}
		//保证可写的至少为len长度
		void ensureWriteableBytes(size_t len)
		{
			if (writableBytes() < len)
				buffer_.resize(writeIndex_ + len);
		}
		//读取的指针
		const uint8_t* readIndexPtr() const 
		{
			return bufferPtr() + readIndex_;
		}
		//恢复到最初的状态
		void resetIndex()
		{
			readIndex_ = 0;
			writeIndex_ = 0;
		}
		//清理读取的index
		void clearReadIndex(uint32_t len)
		{
			if (len < readableBytes())
			{
				readIndex_ += len;
			}
			else
			{
				resetIndex();
			}
		}
		//读取到readBuf中,长度为len
		int readAsString(uint32_t len, string& readBuf)
		{
			if (len > readableBytes())
				len = readableBytes();
			readBuf.assign((const char*)readIndexPtr(), len);
			clearReadIndex(len);
			return len;
		}
		int readAllAsString(string& readBuf)
		{
			return readAsString(readableBytes(), readBuf);
		}
		//读取到data中
		int readAllAsByte(vector<uint8_t>& data)
		{
			return readBytes(data, readableBytes());
		}
		int readBytes(vector<uint8_t>& data, uint32_t len)
		{
			if (len > readableBytes())
				len = readableBytes();
			data.assign(readIndexPtr(), readIndexPtr() + len);
			clearReadIndex(len);
			return len;
		}
		//交换
		void swap(Buffer& rhs);
		//查找crlf
		const char* findCRLF() const
		{
			char kCRLF[] = "\r\n";
			const char* crlf = search((char*)readIndexPtr(), (char*)bufferPtr() + writeIndex_, kCRLF, kCRLF + 2);

			return crlf == ((char*)bufferPtr() + writeIndex_) ? NULL : crlf;
		}
		//清除已经读取的缓冲区直到end
		void retrieveUntil(const char* end)
		{
			if (!(readIndexPtr() <= (uint8_t*)end))
			{
				LOG(ERROR) << "error retrieveUntil";
				return;
			}
			if (!(end <= ((char*)bufferPtr() + writeIndex_)))
			{
				LOG(ERROR) << "error retrieveUntil";
				return;
			}
			clearReadIndex((uint8_t*)end - readIndexPtr());
		}
		
	private:
		uint32_t readIndex_; //读的index
		uint32_t writeIndex_; //写的index
		vector<uint8_t> buffer_; //底层存储-使用vector
		//返回头部指针
		const uint8_t* bufferPtr() const
		{
			return &(*buffer_.begin());
		}
	};
}



#endif
