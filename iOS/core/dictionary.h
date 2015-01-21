#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <map>
#include <vector>
#include <string>

class Dictionary
{
public:

    bool               Insert(const char *text);
    int                InsertAll(const char **text); // insert until null

    void               Clear();

    const char        *GetText(int index)         const;
    int                GetId(const char *text)    const;

private:

    struct CStringComparator
    {
        inline bool operator()(const char *a, const char *b) const { return strcmp(a, b) < 0; }
    };

    typedef std::map<const char *, int, CStringComparator> map_t;

    std::vector<const char *> mText;
    map_t                     mMap;
};

#endif // _DICTIONARY_H