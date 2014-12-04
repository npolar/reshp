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
            
            void calculate_aabb();
            bool contains(const reshp::point&) const;
            bool inside(const reshp::polygon::ring&) const;
            bool intersects(const reshp::polygon::ring&, std::vector<reshp::point>* intersections = NULL) const;
        };
        
        polygon();
        polygon(const reshp::polygon&);
        polygon(const reshp::shp::polygon&);
        
        reshp::aabb aabb;
        std::vector<reshp::polygon::ring> rings;
        
        void calculate_aabb();
        bool inside(const reshp::polygon&) const;
        bool intersects(const reshp::polygon&, std::vector<reshp::point>* intersections = NULL) const;
    };
}

#endif // RESHP_POLYGON_HPP_
