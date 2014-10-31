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

#ifndef RESHP_TRIANGLE_HPP_
#define RESHP_TRIANGLE_HPP_

#include "point.hpp"

namespace reshp
{
    struct triangle
    {
        reshp::point p1, p2, p3;
        
        triangle(const reshp::point&, const reshp::point&, const reshp::point&);
        
        int direction() const; // 0: Colinear, 1: Clockwise, -1: Counter-Clockwise
    };
}

#endif // RESHP_TRIANGLE_HPP_
