// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _STREAMS_IINSTREAM_IOUTSTREAM_H
#define _STREAMS_IINSTREAM_IOUTSTREAM_H

#include <string>
#include <stdio.h>
#include <string.h>

class IStream
{
public:
    virtual ~IStream() {}

    virtual void   close()                                 = 0;
};

class IInStream : public IStream
{
public:

    virtual size_t read(void *buffer, size_t bytes)        = 0;
};

class IOutStream : public IStream
{
public:

    virtual size_t write(const void *buffer, size_t bytes) = 0;
    virtual void   flush()                                 = 0;
};

inline bool write(IOutStream &s, const char *message)
{
    size_t size = strlen(message);
    return s.write(message, size) == size;
}

inline bool write(IOutStream &s, const std::string &message)
{
    return s.write(message.data(), message.size()) == message.size();
}

bool write_format(IOutStream &s, const char *fmt, ...);

// In memory implementations of IInStream and IOutStream
class MemoryInStream : public IInStream
{
    const char *m_buffer;
    size_t      m_size;
    const char *m_ptr;

public:

    MemoryInStream(const void *buffer, size_t size);   
    size_t read(void *u, size_t bytes);
    void   close() {}
};

class MemoryOutStream : public IOutStream
{
    char       *m_buffer;
    size_t      m_size;
    char       *m_ptr;

public:

    MemoryOutStream(void *buffer, size_t size);
    size_t write(const void *u, size_t bytes);
    size_t size() const { return m_ptr - m_buffer; }
    void   close() {}
    void   flush() {}
};

class StringOutStream : public IOutStream
{
    std::string m_str;

public:

    size_t             write(const void *u, size_t bytes) { m_str.append((const char *)u, bytes); return bytes; }
    void               close() {}
    void               flush() {}
    const std::string &str() const { return m_str; }    
};

// Buffered implementations of IInStream and IOutStream
class FileInStream : public IInStream
{
    FILE *m_file;

public:

    FileInStream(FILE *file);
    FileInStream(const char *path);

    size_t read(void *u, size_t bytes);
    void   close();
};

class FileOutStream : public IOutStream
{
    FILE *m_file;

public:

    FileOutStream(FILE *file);
    FileOutStream(const char *path);

    size_t write(const void *u, size_t bytes);
    void   close();
    void   flush();
};

class StdOutStream : public FileOutStream
{
public:
    StdOutStream(FILE *file) : FileOutStream(file) {}
    void close() {}
};

#endif  // _STREAMS_IINSTREAM_IOUTSTREAM_H