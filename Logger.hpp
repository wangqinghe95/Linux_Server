#ifndef LOGGER_HPP__
#define LOGGER_HPP__

#include <string>
#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;
using std::cout;
using std::endl;

enum class LogLevel
{
    TRACE = 0,
    INFO,
    DEBUG,
    WARNING,
    ERROR,
    FATAL
};

/*
 * [time][log_level][file_name:function_name:line]
*/

#define LOG(LEVEL, ...) Logger::getInstance().printLog(__FILE__, LEVEL, __LINE__, __func__, __VA_ARGS__)
#define TRACE(...)  LOG(LogLevel::TRACE, __VA_ARGS__ )
#define INFO(...)   LOG(LogLevel::INFO, __VA_ARGS__ )
#define DEBUG(...)  LOG(LogLevel::DEBUG, __VA_ARGS__ )
#define WARN(...)   LOG(LogLevel::WARNING, __VA_ARGS__ )
#define ERROR(...)  LOG(LogLevel::ERROR, __VA_ARGS__ )
#define FATAL(...)  LOG(LogLevel::FATAL, __VA_ARGS__ )

class Logger
{
public:
    static Logger& getInstance() {
        static Logger logger;
        return logger;
    }

    template<typename... Args>
    void printLog(const string& log_file, LogLevel log_level , const long log_line, const string& log_func, Args... args) {

        string log_message;
        log_message += getCurrentTime();
        log_message += getLogLevel(log_level);
        log_message += getLogLocation(log_file, log_line, log_func);

        ostringstream oss;
        getArgs(oss, args...);
        log_message += oss.str();

        std::cout << log_message;
    }
private:

    string getCurrentTime();
    string getLogLevel(LogLevel log_level);
    string getLogLocation( const string& log_file, const long log_line, const string& log_func);

    template<typename T, typename... Args>
    void getArgs(ostringstream& oss, const T& t, const Args&... args) {
        oss << t << " ";
        getArgs(oss, args...);
    }
    template<typename T>
    void getArgs(ostringstream& oss, const T& t) {
        oss << t << "\n";
    }
};

#endif