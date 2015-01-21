#include "dictionary.h"
#include "strs.h"

bool Dictionary::Insert(const char *text)
{
    if (!text) return false;
    map_t::const_iterator i = mMap.find(text);
    if (i != mMap.end()) return false;
    const char *str = strs_strdup(text);
    mMap[str] = static_cast<int>(mText.size());
    mText.push_back(str);
    return true;
}

int Dictionary::InsertAll(const char **text)
{
    const char **p = text;
    int count = 0;
    while (*p)
    {
        if (Insert(*p))
            count++;
        p++;
    }
    return count;
}

void Dictionary::Clear()
{
    mMap.clear();
    for (std::vector<const char *>::const_iterator i = mText.begin(), e = mText.end(); i != e; ++i)
        delete [] *i;
    mText.clear();
}

const char *Dictionary::GetText(int index) const
{
    return (index < 0 || index >= static_cast<int>(mText.size())) ? 0 : mText[index];
}

int Dictionary::GetId(const char *text) const
{
    if (!text) return -1;
    map_t::const_iterator i = mMap.find(text);
    if (i == mMap.end()) return -1;
    return (*i).second;
}