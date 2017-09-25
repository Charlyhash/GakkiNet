#ifndef __LOG_H__
#define __LOG_H__

#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "Singleton.h"

namespace GakkiNet
{
    class GakkiLog
    {
    private:
		//初始化Glog
		static void InitGLog();
        //捕获core_dump
		static void SignalLogHandle(const char* data, int size);

	public:
		static GakkiLog* single;
		static GakkiLog* getSingle();
		static GakkiLog& getSingleRefer()
		{
			return *getSingle();
		}
	private:
		GakkiLog();
		~GakkiLog();

    };

}

#endif