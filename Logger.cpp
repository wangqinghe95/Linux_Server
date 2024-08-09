#include "Logger.hpp"
#include <ctime>
#include <chrono>
#include <iomanip>

string Logger::getCurrentTime()
{
    time_t now = time(NULL);
    char str_tmp[64] = {0};
    strftime(str_tmp, sizeof(str_tmp), "[%Y-%m-%d %H:%M:%S]",localtime(&now));

    return str_tmp;
}
string Logger::getLogLevel(LogLevel log_level)
{
    string str_level;
    switch (log_level)
    {
    case LogLevel::TRACE:
        str_level = " [TRACE]";
        break;
    case LogLevel::INFO:
        str_level = " [INFO]";
        break;
    case LogLevel::DEBUG:
        str_level = " [DEBUG]";
        break;
    case LogLevel::WARNING:
        str_level = " [WARNING]";
        break;
    case LogLevel::ERROR:
        str_level = " [ERROR]";
        break;
    case LogLevel::FATAL:
        str_level = " [FATAL]";
        break;
    default:
        break;
    }
    return str_level;
}
string Logger::getLogLocation( const string& log_file, const long log_line, const string& log_func)
{
    string str_log_level;
    str_log_level = " [" + log_file + ":" + log_func + ":" + std::to_string(log_line) + "]";
    return str_log_level;
}