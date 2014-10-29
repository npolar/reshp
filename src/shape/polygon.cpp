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

#include "polygon.hpp"
#include <algorithm>

namespace reshp
{
    polygon::ring::ring(const reshp::polygon::ring* parent) :
        parent(parent)
    {
    }
    
    polygon::polygon():
        shape("polygon")
    {
    }
    
    /* TODO:
    bool polygon::inside(const reshp::shape* other) const
    {
    }
    
    bool polygon::intersects(const reshp::shape* other) const
    {
    }
    */
    
    void polygon::update()
    {
        for(unsigned r = 0; r < rings.size(); ++r)
        {
            for(unsigned p = 0; p < rings[r].points.size(); ++p)
            {
                aabb.min.x = std::min(aabb.min.x, rings[r].points[p].x);
                aabb.min.y = std::min(aabb.min.y, rings[r].points[p].y);
                aabb.min.z = std::min(aabb.min.z, rings[r].points[p].z);
                
                aabb.max.x = std::max(aabb.max.x, rings[r].points[p].x);
                aabb.max.y = std::max(aabb.max.y, rings[r].points[p].y);
                aabb.max.z = std::max(aabb.max.z, rings[r].points[p].z);
            }
        }
    }
}
