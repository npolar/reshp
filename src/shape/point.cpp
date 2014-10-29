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

#include "point.hpp"

namespace reshp
{
    point::point(const double x, const double y, const double z) :
        shape("point"),
        x(x),
        y(y),
        z(z)
    {
    }
    
    /* TODO:
    bool point::inside(const reshp::shape* other) const
    {
    }
    
    bool point::intersects(const reshp::shape* other) const
    {
    }
    */
    
    void point::update()
    {
        aabb.min.x = aabb.max.x = x;
        aabb.min.y = aabb.max.y = y;
        aabb.min.z = aabb.max.z = z;
    }
}
