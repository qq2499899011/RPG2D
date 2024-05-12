#include "RPG2Dpch.h"
#include "LogSystem.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace RPG2D {
	//静态成员类外的定义
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());//输出到控制台
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("RPG2D.log", true));//输出到文件

		//设置日志格式 T时间 n记录器名称 v输出内容 l日志级别
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");//控制台格式
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");//文件日志格式

		//从begin 到 end，说明日志也有，控制台也有。日志记录器会将内容输出到所有sink中
		s_CoreLogger = std::make_shared<spdlog::logger>("RPG2D", begin(logSinks), end(logSinks));
		//log注册，保证其他地方可以获取到logger
		spdlog::register_logger(s_CoreLogger);
		//设置日志输出过滤级别，目前最低，表示所有信息都会输出。
		s_CoreLogger->set_level(spdlog::level::trace);
		//指定什么时候更新输出。
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

}

