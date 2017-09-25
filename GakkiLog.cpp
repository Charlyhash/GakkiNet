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
	google::SetStderrLogging(google::INFO); //���ü������google::INFO ����־ͬʱ�������Ļ
	FLAGS_logtostderr = false; //logtostderrΪfalse,����־��log�ļ��С�
	FLAGS_colorlogtostderr = true; //�����������Ļ����־��ʾ��Ӧ��ɫ
	google::SetLogDestination(google::INFO, "./log/INFO_");//���� google::INFO �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::WARNING, "./log/WARNING_");
	google::SetLogDestination(google::ERROR, "./log/ERROR_");
	FLAGS_logbufsecs = 0; //������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
	FLAGS_max_log_size = 100; //�����־��СΪ 100MB
	FLAGS_stop_logging_if_full_disk = true; //�����̱�д��ʱ��ֹͣ��־���
	google::InstallFailureSignalHandler(); //��׽ core dumped
	google::InstallFailureWriter(&SignalLogHandle);//Ĭ�ϲ�׽ SIGSEGV �ź�
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