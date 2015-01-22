// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _READ_FILE_H
#define _READ_FILE_H

#include <string>
#include <sys/stat.h>

namespace file
{

struct buffer_t
{
    void   *data;
    size_t  size;
    
    inline buffer_t(void *data = 0, size_t size = 0) : data(data), size(size) {}
};

buffer_t     read_file(const char *filename);
std::string  read_file_string(const char *filename);
bool         stat(const char *fn, struct _stat *st);
bool         stat(int fd, struct _stat *st);
bool         is_directory(const char *path);
bool         is_directory(int hd);
time_t       time_modified(const char *filename);
time_t       time_modified(int hd);
size_t       file_size(const char *filename);
size_t       file_size(int hd);
bool         path_exists(const char *filename);
bool         path_exists(int hd);
bool         read_only(const char *path);

}

#endif  /** _READ_FILE_H */