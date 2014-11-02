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
    
    bool segment::intersects(const reshp::segment& other) const
    {
        struct triplet
        {
            static int direction(const reshp::point& a, const reshp::point& b, const reshp::point& c)
            {
                int dir = (((b.y - a.y) * (c.x - b.x)) -
                           ((b.x - a.x) * (c.y - b.y)));
                
                // 0: Colinear, 1: Clockwise, -1: Counter-clockwise
                return (dir > 0 ? 1 : (dir < 0 ? -1 : 0));
            }
        };
        
        int aba = triplet::direction(start, end, other.start);
        int abb = triplet::direction(start, end, other.end);
        int baa = triplet::direction(other.start, other.end, start);
        int bab = triplet::direction(other.start, other.end, end);
        
        if(aba != abb && baa != bab)
            return true;
        
        if(!aba && other.start.intersects(*this))
            return true;
        
        if(!abb && other.end.intersects(*this))
            return true;
        
        if(!baa && start.intersects(other))
            return true;
        
        if(!bab && end.intersects(other))
            return true;
        
        return false;
    }
}
