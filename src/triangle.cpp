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

#include "triangle.hpp"

namespace reshp
{
    triangle::triangle(const reshp::point& p1, const reshp::point& p2, const reshp::point& p3) :
        p1(p1),
        p2(p2),
        p3(p3)
    {
    }
    
    int triangle::direction() const
    {
        int dir = (((p2.y - p1.y) * (p3.x - p2.x)) -
                   ((p2.x - p1.x) * (p3.y - p2.y)) -
                   ((p2.z - p1.z) * (p3.z - p2.z)));
                    
        // 0: Colinear, 1: Clockwise, -1: Counter-Clockwise
        return (dir > 0 ? 1 : (dir < 0 ? -1 : 0));
    }
}
