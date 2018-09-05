#include "kjlog.h"
#include <time.h>
Logger::Logger() : fp(LOG_FILE, std::ios::in | std::ios::out) {

}

Logger &Logger::get() {
	static Logger log;
	return log;
}

std::ostream& Logger::getStream() {
	if (fp.is_open()) {
		return fp;
	}
	return std::cout;
}

std::ostream &Logger::log(LoggerType type, const char * file_name, 
						  const char * function_name, int line) {
	std::ostream &os = getStream();
	std::string s;
	switch (type) {
		case LoggerType::LOG_INFO:
		{
			s = "<INFO>";
			break;
		}
		case LoggerType::LOG_DEBUG:
		{
			s = "<DEBUG>";
			break;
		}
		case LoggerType::LOG_ERROR:
		{
			s = "<ERROR>";
			break;
		}
		case LoggerType::LOG_WARN:
		{
			s = "<WARN>";
			break;
		}
		default: { }
	}
	time_t tt = time(NULL);
	char now[30];
#ifdef _WIN32
	ctime_s(now, 30, &tt);
#else
	ctime_r(now, 30, &tt);
#endif
	os << s.c_str() << "---time:" << now << "  " << file_name <<"  "<<  function_name << ":" << line << "  ";
	return  os;
}

Logger::~Logger() {
	fp.close();
}
