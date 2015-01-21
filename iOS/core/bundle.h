#ifndef _BUNDLE_H
#define _BUNDLE_H

#include "core.h"
#include "file.h"

#include <map>

typedef String string_t;

class Bundle
{
public:

    ~Bundle();

    bool        Load(const char *filename);
    const char *GetFileName() const;
    void        Unload();

    const char *GetFile(const char *filename) const;
    int         GetSize(const char *filename) const;

    // temp
    void        Test();

private:

    bool        ReadIndex();

    file::buffer_t                   mData;
    std::map<string_t, const char *> mIndex;
    string_t                         mFilename;
};

#endif // _BUNDLE_H