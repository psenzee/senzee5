#include "streams.h"

#include <stdio.h>
#include <string>
#include <exception>

#include "format.h"

bool write_format(IOutStream &s, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    // this is not acceptable for performance & memory
    std::string formatted = format::format_args(fmt, args);
    va_end(args);
    return s.write(formatted.data(), formatted.size()) == formatted.size();
}

MemoryInStream::MemoryInStream(const void *buffer, size_t size) : m_buffer((const char *)buffer), m_size(size), m_ptr((const char *)buffer)
{
}
    
size_t MemoryInStream::read(void *u, size_t bytes)
{
    size_t stream_size = m_ptr - m_buffer;
    if (stream_size < m_size)
    {
        if (stream_size + bytes >= m_size)
            bytes = m_size - stream_size;
        memcpy(u, m_ptr, bytes);
        m_ptr += bytes;
        return bytes;
    }
    return 0;
}

MemoryOutStream::MemoryOutStream(void *buffer, size_t size) : m_buffer((char *)buffer), m_size(size), m_ptr((char *)buffer)
{
}

size_t MemoryOutStream::write(const void *u, size_t bytes)
{
    size_t stream_size = m_ptr - m_buffer;
    if (stream_size < m_size)
    {
        if (stream_size + bytes >= m_size)
            bytes = m_size - stream_size;
        memcpy(m_ptr, u, bytes);
        m_ptr += bytes;
        return bytes;
    }
    return 0;
}

FileInStream::FileInStream(const char *path) : m_file(0)
{
    m_file = ::fopen(path, "rb");
    if (!m_file)
    {
#ifdef WIN32
        throw std::exception("error: can't open file.");
#else
        throw std::exception();
#endif
    }
}

FileInStream::FileInStream(FILE *file) : m_file(m_file)
{
}

void FileInStream::close()
{
    if (m_file)
        ::fclose(m_file);
    m_file = 0;
}

size_t FileInStream::read(void *u, size_t bytes)
{
    return !m_file ? 0 : (size_t)::fread(u, 1, bytes, m_file);
}

FileOutStream::FileOutStream(const char *path) : m_file(0)
{
    m_file = ::fopen(path, "wb");
    if (!m_file)
#ifdef WIN32
        throw std::exception("error: can't open file.");
#else
        throw std::exception();
#endif
}

FileOutStream::FileOutStream(FILE *file) : m_file(file)
{
}

void FileOutStream::close()
{
    if (m_file)
        ::fclose(m_file);
    m_file = 0;
}

void FileOutStream::flush() 
{
    if (m_file)
        ::fflush(m_file);
}

size_t FileOutStream::write(const void *u, size_t bytes)
{
    return !m_file ? 0 : (size_t)::fwrite(u, 1, bytes, m_file);
}
