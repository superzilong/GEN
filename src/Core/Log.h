#pragma once
#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
	static void Init();
	static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }

private:
	static std::shared_ptr<spdlog::logger> s_logger;
};

#define GEN_LOG_TRACE(...)    Log::GetLogger()->trace(__VA_ARGS__)
#define GEN_LOG_INFO(...)     Log::GetLogger()->info(__VA_ARGS__)
#define GEN_LOG_WARN(...)     Log::GetLogger()->warn(__VA_ARGS__)
#define GEN_LOG_ERROR(...)    Log::GetLogger()->error(__VA_ARGS__)
#define GEN_LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)