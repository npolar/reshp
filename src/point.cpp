/* * * * * * * * * * * * *\
|*             ╔═╗ v0.4  *|
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
    point::point(const reshp::point& other) :
        x(other.x),
        y(other.y),
        z(other.z)
    {
    }
    
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
    
    bool point::operator==(const reshp::point& other) const
    {
        double epsilon = 10e-5, epsilon_neg = (epsilon / -1);
        
        return (((x - other.x) < epsilon) && ((x - other.x) > epsilon_neg)) &&
               (((y - other.y) < epsilon) && ((y - other.y) > epsilon_neg)) &&
               (((z - other.z) < epsilon) && ((z - other.z) > epsilon_neg));
    }
    
    bool point::operator!=(const reshp::point& other) const
    {
        return !(*this == other);
    }
}
