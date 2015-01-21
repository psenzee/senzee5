#include "tuple3f.h"
#include "math.h"
#include "plane.h"

const Plane Plane::INVALID = Plane();

bool Plane::intersect(const Edge &line, point_t &at)
{
    point_t nl    = line.normal();
    float   denom = dot(nl),
            num   = dot(line.a) + dist;
    if (math::eq(denom, 0.f)) return false;
    else at = line.a - nl * (num / denom);
    return true;
}

bool Plane::intersect(const Edge &line, float &t)
{
    point_t nl    = line.normal();
    float   denom = dot(nl),
            num   = dot(line.a) + dist;
    if (math::eq(denom, 0.f)) return false;
    else t = num / denom;
    return true;
}

bool Plane::set(const point_t &a, const point_t &b, const point_t &c)
{
    Tuple3f pa(a), pb(b), pc(c),
            n((pb - pa).cross(pc - pa));        
    float   distance = -n.dot(pa),
            m        =  n.length();
    if (m == 0.0)
        return false;
    m = 1 / m; n *= m; distance *= m;
    set(n, distance);
    return true;
}

bool Plane::intersect(const self &other, Edge &at) const
{
    if (this == &other || math::eq(dot(other.normal), 1.f))
        return false;

    float fn00 = normal.length(), // should be 1!
          fn01 = dot(other.normal),
          fn11 = other.normal.length(), // should be 1!
          det  = fn00 * fn11 - fn01 * fn01;

    if (math::zero(det))
        return false;

    det = 1.f / det;
    float fc0 = (fn11 * dist + fn01 * -other.dist) * det,
          fc1 = (fn00 * other.dist  + fn01 * -dist) * det;
    at.a = normal * fc0 + other.normal * fc1;
    at.b = at.a + normal.cross(other.normal);
    return true;
}

/* Determines if an edge bounded by (x1,y1,z1)->(x2,y2,z2) intersects
 * the plane.
 *
 * If there's an intersection,
 *    the sign of (x1,y1,z1), NEGATIVE or POSITIVE, w.r.t. the plane is
 *    returned with the intersection (ixx,iyy,izz) updated.
 * Otherwise ZERO is returned.
 */
math::Sign Plane::intersect(const point_t &p1, const point_t &p2, point_t &at) const
{
    float      t1, t2;
    math::Sign s1, s2, zero = math::ZERO;

    if      ((s1 = math::sign(t1 = distance(p1))) == zero) return zero;
    else if ((s2 = math::sign(t2 = distance(p2))) == zero) { at = p2; return s1; }
    else if (s1 == s2)                                     return zero;

    return intersectat(p1, p2, at) ? s1 : zero;
}

bool Plane::intersectat(const point_t &p1, const point_t &p2, point_t &at) const // infinite line..
{
    float denom = 0.f;
    // intersection point
    point_t dp(p2 - p1);
    if (math::eq(denom = dot(dp), 0.f))
        return false;
    at = p1 + (dp * (-distance(p1) / denom));
    return true;
}
/*
bool Plane::intersect(const self &b, const self &c, point_t &at) const
{
    Edge ln;
    if (!intersect(b, ln)) return false;
    return c.intersectat(ln.a, ln.b, at);
}
 */

bool Plane::normalize()
{
    double da   = normal.x, db = normal.y, dc = normal.z, dd = dist,
           lsq  = da * da + db * db + dc * dc,
           m;
    if (!math::eq(lsq, 1.0))
    {
        if (lsq == 0.0)
            return false;
        m = 1.0 / sqrt(lsq);
        da *= m; db *= m; dc *= m; dd *= m;
        _set((float)da, (float)db, (float)dc, (float)dd);
    }
    return true;
}
