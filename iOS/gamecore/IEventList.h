#ifndef _IEVENTLIST_H
#define _IEVENTLIST_H

class Event;

class IEventList
{
public:
    virtual ~IEventList() {}

    virtual Event **GetEvents()           = 0;
    virtual int     GetEventCount() const = 0;
};

#endif // _IEVENTLIST_H
