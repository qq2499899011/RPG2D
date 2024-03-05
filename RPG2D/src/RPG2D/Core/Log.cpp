#include "RPG2Dpch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace RPG2D {
	//��̬��Ա����Ķ���
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());//���������̨
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("RPG2D.log", true));//������ļ�

		//������־��ʽ
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");//����̨��ʽ
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");//�ļ���־��ʽ

		//��begin �� end��˵����־Ҳ�У���ʼ��Ҳ�С�
		s_CoreLogger = std::make_shared<spdlog::logger>("RPG2D", begin(logSinks), end(logSinks));
		//logע�ᣬ��֤�����ط����Ի�ȡ��logger
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

}

