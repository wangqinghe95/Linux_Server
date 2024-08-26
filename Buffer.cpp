#include "Buffer.hpp"
#include <iostream>
#include <cstring>

Buffer::Buffer()
{

}

Buffer::~Buffer()
{

}


void Buffer::append(const char* _str, int _size)
{
    for(int i = 0; i < _size && _str[i] != '\0'; ++i) {
         m_buffer_string.push_back(_str[i]);
    }
}

ssize_t Buffer::size()
{
    return m_buffer_string.size();
}

const char* Buffer::c_str()
{
    return m_buffer_string.c_str();
}

void Buffer::clear()
{
    m_buffer_string.clear();
}

void Buffer::getline()
{
    clear();
    std::getline(std::cin, m_buffer_string);
}
