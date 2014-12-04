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

#include "segment.hpp"

namespace reshp
{
    segment::segment(const reshp::point& start, const reshp::point& end) :
        start(start),
        end(end)
    {
    }
    
    bool segment::intersects(const reshp::segment& other, reshp::point* intersection) const
    {
        // TODO: Division by zero handling?
        
        double ax = end.x - start.x;
        double ay = end.y - start.y;
        double bx = other.end.x - other.start.x;
        double by = other.end.y - other.start.y;
        double s = ((-ay * (start.x - other.start.x)) + (ax * (start.y - other.start.y))) / ((-bx * ay) + (ax * by));
        double t = (( bx * (start.y - other.start.y)) - (by * (start.x - other.start.x))) / ((-bx * ay) + (ax * by));
        
        if(s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
        {
            if(intersection)
            {
                intersection->x = start.x + (t * ax);
                intersection->y = start.y + (t * ay);
            }
            
            return true;
        }
        
        return false;
    }
}
