#pragma once
#include <fstream>
#include <iostream>
#define LOG_FILE " " //"../logfile.log"
namespace KeaneJ {
	enum class LoggerType
	{
		LOG_INFO = 1,
		LOG_DEBUG = 2,
		LOG_ERROR = 3,
		LOG_WARN = 4
	};
	class Logger {
	public:
		static Logger& get();
		std::ostream &getStream();
		std::ostream &log(LoggerType type, const char *file_name, const char *function_name, int line);
		~Logger();

	private:
		Logger();

	private:
		std::fstream fp;
	};
}
#define KJLOG_INFO  KeaneJ::Logger::get().log(KeaneJ::LoggerType::LOG_INFO,  __FILE__, __FUNCTION__, __LINE__)
#define KJLOG_DEBUG  KeaneJ::Logger::get().log(KeaneJ::LoggerType::LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__)
#define KJLOG_ERROR  KeaneJ::Logger::get().log(KeaneJ::LoggerType::LOG_ERROR, __FILE__, __FUNCTION__, __LINE__)
#define KJLOG_WARN  KeaneJ::Logger::get().log(KeaneJ::LoggerType::LOG_WARN, __FILE__, __FUNCTION__, __LINE__)
