/* * * * * * * * * * * * *\
|*             ╔═╗ v0.3  *|
|* ╔═╦═╦═══╦═══╣ ╚═╦═╦═╗ *|
|* ║ ╔═╣ '╔╬═╗╚╣ ║ ║  '║ *|
|* ╚═╝ ╚═══╩═══╩═╩═╣ ╔═╝ *|
|* * * * * * * * * ╚═╝ * *|
|* Manipulation tool for *|
|*    ESRI Shapefiles    *|
|* * * * * * * * * * * * *|
|* http://www.npolar.no/ *|
\* * * * * * * * * * * * */

#include "aabb.hpp"
#include <cfloat>
#include <algorithm>

namespace reshp
{
    aabb::aabb() :
        min( DBL_MAX,  DBL_MAX, 0.0),
        max(-DBL_MAX, -DBL_MAX, 0.0)
    {
    }
    
    aabb::aabb(const reshp::point& min, const reshp::point& max) :
        min(min),
        max(max)
    {
        this->min.x = std::min(min.x, max.x);
        this->min.y = std::min(min.x, max.x);
        this->min.z = std::min(min.x, max.x);
        
        this->max.x = std::max(min.x, max.x);
        this->max.y = std::max(min.y, max.y);
        this->max.z = std::max(min.z, max.z);
    }
    
    void aabb::initialize()
    {
        min = reshp::point( DBL_MAX,  DBL_MAX, 0.0);
        max = reshp::point(-DBL_MAX, -DBL_MAX, 0.0);
    }
    
    bool aabb::inside(const reshp::aabb& other) const
    {
        if(min.z != 0.0 || max.z != 0.0 || other.min.z != 0.0 || other.max.z != 0.0)
        {
            return ((other.min.x < min.x && other.max.x > max.x) &&
                    (other.min.y < min.y && other.max.y > max.y) &&
                    (other.min.z < min.z && other.max.z > max.z));
        }
        
        return ((other.min.x < min.x && other.max.x > max.x) &&
                (other.min.y < min.y && other.max.y > max.y));
    }
    
    bool aabb::intersects(const reshp::aabb& other) const
    {
        if(min.z != 0.0 || max.z != 0.0 || other.min.z != 0.0 || other.max.z != 0.0)
        {
            return (((other.min.x > min.x && other.min.x < max.x) || (other.max.x > min.x && other.max.x < max.x)) &&
                    ((other.min.y > min.y && other.min.y < max.y) || (other.max.y > min.y && other.max.y < max.y)) &&
                    ((other.min.z > min.z && other.min.z < max.z) || (other.max.z > min.z && other.max.z < max.z)));
        }
        
        return (((other.min.x > min.x && other.min.x < max.x) || (other.max.x > min.x && other.max.x < max.x)) && 
                ((other.min.y > min.y && other.min.y < max.y) || (other.max.y > min.y && other.max.y < max.y)));
    }
    
    void aabb::operator>> (double (&xy)[4])
    {
        xy[0] = min.x;
        xy[1] = min.y;
        xy[2] = max.x;
        xy[3] = max.y;
    }
    
    void aabb::operator>> (double (&xyz)[6])
    {
        xyz[0] = min.x;
        xyz[1] = min.y;
        xyz[2] = min.z;
        xyz[3] = max.x;
        xyz[4] = max.y;
        xyz[5] = max.z;
    }
}
