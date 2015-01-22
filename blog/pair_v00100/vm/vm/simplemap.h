// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SIMPLEMAP_H
#define _SIMPLEMAP_H

#include <string>

class SimpleMap
{
public:
    struct Entry
    {
        std::string  name;
        int          index;
    };

    SimpleMap() : m_count(0) {};

    Entry *set(const std::string &name, int id)
    {
        Entry *e = find(name.c_str());
        if (e) e->index = id;
        else e = append(name, id);
        return e;
    }

    Entry *set(const char *name, int id)
    {
        name = name ? name : "";
        Entry *e = find(name);
        if (e) e->index = id;
        else e = append(std::string(name), id);
        return e;
    }

    void clear()
    {
        m_count = 0;
    }

    Entry *append(const std::string &name, int id)
    {
        Entry *e = &m_index[m_count++];
        e->name = name;
        e->index = id;
        return e;
    }

    Entry *find(const std::string &name)
    {
        return find(name.c_str());
    }

    Entry *find(const char *name)
    {
        for (unsigned i = 0; i < m_count; i++)
            if (strcmp(m_index[i].name.c_str(), name) == 0)
                return &m_index[i];
        return 0;
    }

private:

    enum { MAX_ENTRIES = 1024 };

    Entry    m_index[MAX_ENTRIES];
    unsigned m_count;
};

#endif  // _SIMPLEMAP_H