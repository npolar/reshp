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

#ifndef RESHP_SHAPE_POLYGON_HPP_
#define RESHP_SHAPE_POLYGON_HPP_

#include "../shape.hpp"
#include "point.hpp"
#include <cstdlib>
#include <vector>

namespace reshp
{
    struct polygon : public shape
    {
        struct ring
        {
            ring(const reshp::polygon::ring* parent = NULL);
            
            const reshp::polygon::ring* parent;
            std::vector<reshp::point> points;
        };
        
        polygon();
        
        std::vector<reshp::polygon::ring> rings;
        
        /* TODO:
        bool inside(const reshp::shape*) const;
        bool intersects(const reshp::shape*) const;
        */
        
        void update();
    };   
}

#endif // RESHP_SHAPE_POLYGON_HPP_
