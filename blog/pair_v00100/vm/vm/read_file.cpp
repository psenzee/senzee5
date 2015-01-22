// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "read_file.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace file
{

enum { MAX_PATH = 1024 };

buffer_t read_file(const char *filename)
{
    buffer_t data;
	if (filename)
	{
	    int handle = ::open(filename, O_RDONLY | O_BINARY);
	    if (handle >= 0)
	    {
            data.size = (size_t)::filelength(handle);
            data.data = new char [data.size + 1]; // the + 1 we null terminate, convenient if it's a string.
            size_t sz = ::read(handle, data.data, (unsigned)data.size);
            if (sz != data.size)
            {
                delete [] data.data;
                data.data = 0;
                data.size = 0;
            }
            ((char *)data.data)[sz] = 0; // null terminate
            ::close(handle);
		}
	}
	return data;
}

std::string read_file_string(const char *filename)
{
    buffer_t data = read_file(filename);
    std::string s((const char *)data.data, data.size);
    delete [] (char *)data.data;
    return s;
}
/*
bool stat(const char *fn, struct _stat *st)
{
    if (!fn || !*fn || !st)
        return false;
    // see MSDN Q168439
    size_t len = strlen(fn);
    char str[MAX_PATH] = { 0 };
    strcpy(str, fn);
    if (str[len - 1] == '\\')
        str[len - 1] = 0;
    memset(st, 0, sizeof(struct _stat));
    return _stat(str, st) == 0;
}

bool stat(int fd, struct _stat *st)
{
    if (!st) return false;
    memset(st, 0, sizeof(struct _stat));
    return _fstat(fd, st) == 0;
}

bool is_directory(const char *path)
{
    if (!path || !*path) return false;
    struct _stat st;
    return stat(path, &st) && (st.st_mode & _S_IFDIR) > 0;
}

bool is_directory(int fd)
{
    if (!fd) return false;
    struct _stat st;
    return stat(fd, &st) && (st.st_mode & _S_IFDIR) > 0;
}

time_t time_modified(const char *fn)
{
    if (!fn || !*fn) return 0;
    struct _stat st;
    return !stat(fn, &st) ? 0 : st.st_mtime;
}

time_t time_modified(int fd)
{
    if (!fd) return 0;
    struct _stat st;
    return !stat(fd, &st) ? 0 : st.st_mtime;
}

bool path_exists(const char *path)
{
    return _access(path, 0) == 0;
}

bool path_exists(int fd)
{
    struct _stat st;
    return fd && stat(fd, &st);
}

bool read_only(const char *path)
{
    return _access(path, 4) == 0;
}

size_t file_size(const char *fn)
{
    struct _stat st;
    return fn && *fn && stat(fn, &st) ? st.st_size : 0;
}

size_t file_size(int fd)
{
    struct _stat st;
    return fd && stat(fd, &st) ? st.st_size : 0;
}
*/
}