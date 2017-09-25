#ifndef __BUFFER_H__
#define __BUFFER_H__
/*
Buffer�ࣺ������Ļ������ķ�װ�������̰߳�ȫ�ġ�
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
		static const uint32_t InitSize; //Ĭ�ϴ�С
		//���캯��
		explicit Buffer(uint32_t size = InitSize):readIndex_(0), writeIndex_(0), buffer_(size) {}
		//��������
		~Buffer() {}
		int readFromIO(int fd, int& errorNo);
		//��д�Ĵ�С
		uint32_t writableBytes()
		{
			return buffer_.size() - writeIndex_; 
		}
		//�ɶ��Ĵ�С
		uint32_t readableBytes() const
		{
			return writeIndex_ - readIndex_;
		}
		//�ж��Ƿ�ɶ�-�ɶ����ֽڴ���0
		bool isReadable()
		{
			return readableBytes() > 0 ? true : false;
		}
		//�ж��Ƿ�Ϊ��
		bool isEmpty()
		{
			return !isReadable();
		}
		//���һ��char*��������
		void append(const char* data, size_t len)
		{
			ensureWriteableBytes(len);
			copy(data, data + len, buffer_.begin() + writeIndex_);
			writeIndex_ += len;
		}
		//���string��������
		void append(const string& str)
		{
			const char* data = &(*str.begin());
			append(data, str.size());
		}
		//��֤��д������Ϊlen����
		void ensureWriteableBytes(size_t len)
		{
			if (writableBytes() < len)
				buffer_.resize(writeIndex_ + len);
		}
		//��ȡ��ָ��
		const uint8_t* readIndexPtr() const 
		{
			return bufferPtr() + readIndex_;
		}
		//�ָ��������״̬
		void resetIndex()
		{
			readIndex_ = 0;
			writeIndex_ = 0;
		}
		//�����ȡ��index
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
		//��ȡ��readBuf��,����Ϊlen
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
		//��ȡ��data��
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
		//����
		void swap(Buffer& rhs);
		//����crlf
		const char* findCRLF() const
		{
			char kCRLF[] = "\r\n";
			const char* crlf = search((char*)readIndexPtr(), (char*)bufferPtr() + writeIndex_, kCRLF, kCRLF + 2);

			return crlf == ((char*)bufferPtr() + writeIndex_) ? NULL : crlf;
		}
		//����Ѿ���ȡ�Ļ�����ֱ��end
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
		uint32_t readIndex_; //����index
		uint32_t writeIndex_; //д��index
		vector<uint8_t> buffer_; //�ײ�洢-ʹ��vector
		//����ͷ��ָ��
		const uint8_t* bufferPtr() const
		{
			return &(*buffer_.begin());
		}
	};
}



#endif
