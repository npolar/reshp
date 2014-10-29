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

#include "shape.hpp"
#include <cfloat>
#include <cstdio>

namespace reshp
{
    aabb::aabb()
    {
        min.x = min.y = DBL_MAX;
        max.x = max.y = DBL_MIN;
        min.z = max.z = 0.0;
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
    
    shape::shape(const char* type) :
        type(type)
    {
    }
    
    shape::~shape()
    {
    }
            
    bool shape::inside(const reshp::shape*) const
    {
        fprintf(stderr, "undefined inside-detection for shapetype: %s\n", type);
        return false;
    }
    
    bool shape::intersects(const reshp::shape*) const
    {
        fprintf(stderr, "undefined intersects-dection for shapetype: %s\n", type);
        return false;
    }
    
    void shape::update()
    {
        fprintf(stderr, "undefined shape-updater for shapetype: %s\n", type);
    }
}
