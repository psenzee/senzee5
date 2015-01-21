#ifndef _PATHS_H
#define _PATHS_H

#include <string>

enum { MAX_PATH = 1024 };

void        make_path(const char *dir, const char *file, std::string &path);
void        change_extension(const char *filename, const char *ext, const char *newext, char *buffer);
void        split_path(const char *filepath, char *path, char *file);
std::string get_filename(const char *filename, bool extension);
std::string get_path(const char *filename);
std::string add_filename_suffix(const char *filename, const char *append);

#endif // _PATHS_H
