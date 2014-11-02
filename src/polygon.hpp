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

#ifndef RESHP_POLYGON_HPP_
#define RESHP_POLYGON_HPP_

#include "point.hpp"
#include "aabb.hpp"
#include "shp.hpp"
#include <vector>

namespace reshp
{
    struct polygon
    {
        struct ring
        {
            ring();
            
            reshp::aabb aabb;
            enum { outer, inner } type;
            std::vector<reshp::point> points;
            
            bool intersects(const reshp::polygon::ring&) const;
        };
        
        polygon();
        polygon(const reshp::shp::polygon&);
        
        reshp::aabb aabb;
        std::vector<reshp::polygon::ring> rings;
        
        bool intersects(const reshp::polygon&) const;
    };
}

#endif // RESHP_POLYGON_HPP_
