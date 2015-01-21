#ifndef _PLANE_H
#define _PLANE_H

#include "tuple3f.h"
#include "tuple4f.h"
#include "mathcore.h"

#include <exception>

class PlaneException : public std::exception
{
public:
    PlaneException(const char *message) 
#ifdef WIN32
        : std::exception(message)
#else
        : std::exception()
#endif
        {}
};

class PlaneNormalizeException : public PlaneException
{
public:
    PlaneNormalizeException(const char *message) : PlaneException(message) {}
};

class PlaneCreateException : public PlaneException
{
public:
    PlaneCreateException(const char *message) : PlaneException(message) {}
};

class Edge
{
public:
    Tuple3f a,
            b;
            
    inline void    set(const Tuple3f &a, const Tuple3f &b)       { this->a = a; this->b = b; }
    inline Tuple3f normal()                                const { return (b - a).normalize(); }
    inline Tuple3f direction()                             const { return b - a; }
};

class Plane
{
public:

    typedef Tuple3f       point_t;
    typedef Plane         self;

    point_t normal;
    float   dist;

    inline Plane()                                                     { _set(0.f, 0.f, 0.f, 0.f); } // force invalid..    
    inline Plane(float a, float b, float c, float d)                   { set(a, b, c, d); }
    inline Plane(const Tuple3f &n, float d)                            { set(n, d); } // assumed normalized - make private?
    inline Plane(const point_t &a, const point_t &b, const point_t &c) { set(a, b, c);    }

    // operators
    inline bool operator<(const self &p) const
    {
        if (normal.exact_lt(p.normal)) return true;
        if (p.normal.exact_lt(normal)) return false;
        return math::lt(dist, p.dist);
    }
    inline bool operator>(const self &p) const
    {
        if (normal.exact_gt(p.normal)) return true;
        if (p.normal.exact_gt(normal)) return false;
        return math::gt(dist, p.dist);
    }
    inline bool operator==(const self &p) const
    {
        return (normal.exact_eq(p.normal) && math::eq(dist, p.dist));
    }
    inline bool operator!=(const self &p) const
    {
       return (normal.exact_neq(p.normal) || !math::eq(dist, p.dist));
    }

    inline Plane operator-() const
    {
        return Plane(-normal, -dist);
    }
    inline float dot(const point_t &p) const
    {
        return normal.x * p.x + normal.y * p.y + normal.z * p.z;
    }
    
    inline Tuple4f vector() const
    {
        return Tuple4f(normal.x, normal.y, normal.z, dist);
    }
    
    inline float distance(const point_t &p) const
    {
        return dot(p) + dist;
    }
    inline bool equalOrOpposite(const self &p) const
    {
        return p == *this || p == -*this;
    }
    inline bool inHalfSpace(const point_t &p) const
    {
        return distance(p) < 0;// but bsp uses neg as in hs
    }
    inline bool straddle(const point_t &a, const point_t &b) const
    {
        return math::sign(distance(a)) != math::sign(distance(b));
    }
    inline bool straddle(const Edge &line) const
    {
        return straddle(line.a, line.b);
    }
    inline void set(const Tuple3f &n, float d) // assumes normalized - make private?
    {
        normal = n;
        dist   = d;
    }
    inline void set(float a, float b, float c, float d)
    {
        _set(a, b, c, d);
        normalize();
    }    
    inline point_t member() const
    {
        return normal * -dist; // $TODO i don't think dist should be neg here..
    }
    inline void flip()
    {
        normal = -normal; dist = -dist;
    }    
    inline float angle(const self &other)
    {
        return acosf(dot(other.normal));
    }    
    
    bool set(const point_t &a, const point_t &b, const point_t &c);
    
    bool intersect(const Edge &line, point_t &at);
    bool intersect(const Edge &line, float &t);
    
    bool intersect(const self &other, Edge &at) const;

    /* Determines if an edge bounded by (x1,y1,z1)->(x2,y2,z2) intersects
     * the plane.
     *
     * If there's an intersection,
     *    the sign of (x1,y1,z1), NEGATIVE or POSITIVE, w.r.t. the plane is
     *    returned with the intersection (ixx,iyy,izz) updated.
     * Otherwise ZERO is returned.
     */
    math::Sign intersect(const point_t &p1, const point_t &p2, point_t &at) const;
    bool       intersectat(const point_t &p1, const point_t &p2, point_t &at) const; // infinite line..
  //bool       intersect(const self &b, const self &c, point_t &at) const;

    bool       normalize();
    
    bool       valid() const { return !math::eq(normal.lengthsq(), 0.f); }
    
    static const Plane INVALID;
    
private:
    
    // don't normalize
    inline void _set(float a, float b, float c, float d)
    {
        normal = point_t(a, b, c); dist = d;
    }   
};

#endif  // _PLANE_H
