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

#ifndef RESHP_SHAPE_POLYLINE_HPP_
#define RESHP_SHAPE_POLYLINE_HPP_

#include "../shape.hpp"
#include "point.hpp"
#include <vector>

namespace reshp
{
    struct polyline : public shape
    {
        struct ring
        {
            std::vector<reshp::point> points;
        };
        
        polyline();
        
        std::vector<reshp::polyline::ring> rings;
        
        /* TODO:
        bool inside(const reshp::shape*) const;
        bool intersects(const reshp::shape*) const;
        */
        
        void update();
    };   
}

#endif // RESHP_SHAPE_POLYLINE_HPP_
