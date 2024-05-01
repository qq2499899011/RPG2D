#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace RPG2D {
	//HACK:临时将ref放在这里
	template<typename T>
	using Ref = std::shared_ptr<T>;
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		//静态变量的声明
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define RPG2D_CORE_TRACE(...)    ::RPG2D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RPG2D_CORE_INFO(...)     ::RPG2D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RPG2D_CORE_WARN(...)     ::RPG2D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RPG2D_CORE_ERROR(...)    ::RPG2D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RPG2D_CORE_CRITICAL(...) ::RPG2D::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RPG2D_TRACE(...)         ::RPG2D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RPG2D_INFO(...)          ::RPG2D::Log::GetClientLogger()->info(__VA_ARGS__)
#define RPG2D_WARN(...)          ::RPG2D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RPG2D_ERROR(...)         ::RPG2D::Log::GetClientLogger()->error(__VA_ARGS__)
#define RPG2D_CRITICAL(...)      ::RPG2D::Log::GetClientLogger()->critical(__VA_ARGS__)
