#include "GakkiLog.h"
using namespace GakkiNet;

GakkiLog* GakkiLog::single = new GakkiLog;

GakkiLog::GakkiLog()
{
	InitGLog();
}

GakkiLog::~GakkiLog()
{
	google::ShutdownGoogleLogging();
}

void GakkiLog::InitGLog()
{
	mkdir("./log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	google::InitGoogleLogging("GAKKI");
	google::SetStderrLogging(google::INFO); //设置级别高于google::INFO 的日志同时输出到屏幕
	FLAGS_logtostderr = false; //logtostderr为false,则将日志到log文件中。
	FLAGS_colorlogtostderr = true; //设置输出到屏幕的日志显示相应颜色
	google::SetLogDestination(google::INFO, "./log/INFO_");//设置 google::INFO 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::WARNING, "./log/WARNING_");
	google::SetLogDestination(google::ERROR, "./log/ERROR_");
	FLAGS_logbufsecs = 0; //缓冲日志输出，默认为30秒，此处改为立即输出
	FLAGS_max_log_size = 100; //最大日志大小为 100MB
	FLAGS_stop_logging_if_full_disk = true; //当磁盘被写满时，停止日志输出
	google::InstallFailureSignalHandler(); //捕捉 core dumped
	google::InstallFailureWriter(&SignalLogHandle);//默认捕捉 SIGSEGV 信号
}

void GakkiLog::SignalLogHandle(const char* data, int size)
{
	std::ofstream fs("glog_dump.log", std::ios::app);
	std::string str = std::string(data, size);
	fs << str;
	fs.close();
}

GakkiLog* GakkiLog::getSingle()
{
	return single;
}