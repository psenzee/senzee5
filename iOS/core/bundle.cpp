#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <assert.h>

#include "bundle.h"
#include "file.h"
#include "paths.h"

struct BundleIndexEntry
{
    size_t           entrySize;
    size_t           fileOffset;
//  char             string[entrySize - 8];
};

struct BundleIndexHeader
{
    char             tag[4]; // = "INDX"
    size_t           size;
    size_t           entryCount;
//  IndexEntry       entries[entryCount];
};

struct BundleFile
{
    char             tag[4];
    size_t           size;
//  char             data[size - 4]; // ...
};

static int GetSizeFromFileOffset(const char *file)
{
    if (!file) return -1;
    return *(const int *)(file - sizeof(int)) - sizeof(int);
}

static bool Validate(const char *file)
{
    if (!file) return false;
    char tag[] = "FILE";
    return memcmp(file - 8, tag, 4) == 0 && GetSizeFromFileOffset(file) >= 0;
}

const char *Bundle::GetFileName() const
{
    return mFilename.c_str();
}

void Bundle::Test()
{
    for (std::map<string_t, const char *>::const_iterator i = mIndex.begin(), e = mIndex.end(); i != e; ++i)
    {
        printf("%p:%d [%s]\n", (*i).second, GetSizeFromFileOffset((*i).second), (*i).first.c_str());
    }
}

void Bundle::Unload()
{
    mFilename = "";
    if (mData.data)
        delete [] (char *)mData.data;
    mData.data = 0;
    mData.size = 0;
    mIndex.clear();
}

Bundle::~Bundle()
{
    Unload();
}

bool Bundle::Load(const char *filename)
{
    Unload();
    mFilename = filename;
    mData = file::read_file(filename);
    if (!ReadIndex())
    {
        Unload();
        return false;
    }
    return true;
}

const char *Bundle::GetFile(const char *filename) const
{ 
    std::map<string_t, const char *>::const_iterator found = mIndex.find(filename);
    if (found == mIndex.end())
        return 0;
    if (!Validate((*found).second))
    {
        printf("Bundle for file '%s' appears corrupt!\n", filename);
        return 0;
    }
    return (*found).second;
}

int Bundle::GetSize(const char *filename) const
{ 
    return GetSizeFromFileOffset(GetFile(filename));
}

bool Bundle::ReadIndex()
{
    BundleIndexHeader header;
    memcpy(&header, mData.data, sizeof(header));
    if (memcmp(header.tag, "INDX", sizeof(header.tag)) != 0)
    {
        // failure..
        return false;
    }
    char *p = (char *)mData.data + sizeof(header);
    char indexName[1024];
    for (int i = 0; i < header.entryCount; i++)
    {
        BundleIndexEntry entry;
        memcpy(&entry, p, sizeof(entry));
        memset(indexName, 0, sizeof(indexName));
        memcpy(indexName, p + sizeof(entry), entry.entrySize - sizeof(size_t));
        //printf("name: %s\n", indexName);
        p += entry.entrySize + sizeof(size_t);
        mIndex[indexName] = (char *)mData.data + entry.fileOffset + sizeof(size_t) + sizeof(size_t);
    }
    return true;
}