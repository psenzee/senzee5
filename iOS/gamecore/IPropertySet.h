#ifndef _IPROPERTYSET_H
#define _IPROPERTYSET_H

class IPropertySet
{
public:
    virtual ~IPropertySet() {}

    virtual bool        SetProperty(const char *key, const char *value) = 0;
    virtual const char *GetProperty(const char *key) const              = 0;
};

#endif // _IPROPERTYSET_H
