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

#ifndef RESHP_SHAPE_POINT_HPP_
#define RESHP_SHAPE_POINT_HPP_

#include "../shape.hpp"

namespace reshp
{
    struct point : public shape
    {
        double x, y, z;
        
        point(const double x = 0.0, const double y = 0.0, const double z = 0.0);
        
        /* TODO:
        bool inside(const reshp::shape*) const;
        bool intersects(const reshp::shape*) const;
        */
        
        void update();
    };   
}

#endif // RESHP_SHAPE_POINT_HPP_
