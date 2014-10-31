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
#include "segment.hpp"
#include <algorithm>

namespace reshp
{
    point::point(const double x, const double y, const double z) :
        x(x),
        y(y),
        z(z)
    {
    }
    
    bool point::intersects(const reshp::segment& segment) const
    {
        return ((segment.end.x <= std::max(segment.start.x, x) && segment.end.x >= std::min(segment.start.x, x)) &&
                (segment.end.y <= std::max(segment.start.y, y) && segment.end.y >= std::min(segment.start.y, y)) &&
                (segment.end.z <= std::max(segment.start.z, z) && segment.end.z >= std::min(segment.start.z, z)));
    }
}
