#ifndef _ICOLLIDABLE_H
#define _ICOLLIDABLE_H

class Tuple3f;

class ICollidable
{
public:
    
    enum Classification { CLASS_IN = -1, CLASS_OUT = 1, CLASS_ON = 0, CLASS_ERROR = -2, CLASS_UNKNOWN = -3 };
    
    virtual ~ICollidable() {}
/*
    virtual Classification ClassifyPoint    (const Tuple3f &point, Tuple3f &normal)                             const = 0;
    virtual Classification ClassifySphere   (const Tuple3f &point, double radius, Tuple3f &at) const = 0;
    virtual bool           CollisionOccurred(const Tuple3f &from, const Tuple3f &to, Tuple3f &normal)           const = 0;
*/
    virtual void           SetIdentifier(const char *name) = 0;
    virtual const char    *GetIdentifier() const           = 0;
};

#endif // _ICOLLIDABLE_H
