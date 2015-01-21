#include "paths.h"
#include "strs_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

void change_extension(const char *filename, const char *ext, const char *newext, char *buffer)
{
    size_t fnsz = strlen(filename);
    strcpy(buffer, filename);
    if (strs_ends(filename, ext, true))
        fnsz -= strlen(ext);
    strcpy(buffer + fnsz, newext);
}

void make_path(const char *dir, const char *file, std::string &path)
{
    path = (dir ? dir : "");
    if (!path.empty())
    {
        char ch = path[path.size() - 1];
        if (ch != '\\' && ch != '/')
#ifdef WIN32
            path += "\\";
#else
            path += "/";
#endif
    }
    path += (file ? file : "");
    for (size_t i = 0, sz = path.size(); i < sz; i++)
#ifdef WIN32
        if (path[i] == '/')
            path[i] = '\\';
#else
        if (path[i] == '\\')
            path[i] = '/';
#endif
}

void split_path(const char *filepath, char *path, char *file)
{
    const char *p = filepath + strlen(filepath), *end = p;
    while (p > filepath && *p != '/' && *p != '\\')
        p--;
    *path = 0; *file = 0;
    if (*p == '/' || *p == '\\')
        p++;
    if (p != filepath)
    {
        // has a path
        memcpy(path, filepath, p - filepath);
        path[p - filepath] = 0;
    }
    if (p != end)
    {
        // has a filename
        memcpy(file, p, end - p);
        file[end - p] = 0;
    }
}

/* // $TODO
void split_path(const char *filepath, char *path, char *file, char *ext)
{
    const char *p = filepath + strlen(filepath), *end = p;
    while (p > filepath && *p != '/' && *p != '\\')
        p--;
    *path = 0; *file = 0;
    if (*p == '/' || *p == '\\')
        p++;
    if (p != filepath)
    {
        // has a path
        memcpy(path, filepath, p - filepath);
        path[p - filepath] = 0;
    }
    if (p != end)
    {
        // has a filename
        memcpy(file, p, end - p);
        file[end - p] = 0;
    }
}
*/

std::string get_filename(const char *filename, bool extension)
{
    if (!filename || !*filename)
        return "";
    char fname[MAX_PATH];
    strcpy(fname, filename);
    if (!extension)
    {
        char *ext = strrchr(fname, '.');
        if (ext) *ext = 0;
    }
    char *start = strchr(fname, '/');
    if (!start)
        start = strchr(fname, '\\');
    if (!start)
        start = fname;
    return start;
}

std::string get_path(const char *filename)
{
    if (!filename || !*filename)
        return "";
    char fname[MAX_PATH];
    strcpy(fname, filename);
    char *end = strchr(fname, '/');
    if (!end)
        end = strchr(fname, '\\');
    if (end)
        *(++end) = 0;
    else
        *fname = 0;
    return fname;
}

std::string add_filename_suffix(const char *filename, const char *append)
{
    if (!append)
        return !filename ? "" : filename;
    if (!filename)
        return append;
    char fname[MAX_PATH], buffer[MAX_PATH];
    strcpy(fname, filename);
    char *ext = strrchr(fname, '.');
    if (!ext)
    {
        sprintf(buffer, "%s%s", fname, append);
        return buffer;
    }
    if (ext) { *ext = 0; ext++; }
    sprintf(buffer, "%s%s.%s", fname, append, ext);
    return buffer;
}