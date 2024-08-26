#ifndef BUFFER_HPP__
#define BUFFER_HPP__

#include <string>

class Buffer
{
private:
    std::string m_buffer_string;
public:
    Buffer();
    ~Buffer();

    void append(const char* _str, int size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
};

#endif // !BUFFER_HPP__#define 