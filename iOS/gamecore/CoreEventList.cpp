#include "CoreEventList.h"

NullEventList NullEventList::instance;

CoreEventList::CoreEventList()
{
}

CoreEventList::CoreEventList(const std::vector<Event *> &events) : mEvents(events)
{
}

Event **CoreEventList::GetEvents()
{
    return static_cast<Event **>(&mEvents[0]);
}

int CoreEventList::GetEventCount() const
{
    return (int)mEvents.size();
}