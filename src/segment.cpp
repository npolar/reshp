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
#include "triangle.hpp"

namespace reshp
{
    segment::segment(const reshp::point& start, const reshp::point& end) :
        start(start),
        end(end)
    {
    }
    
    bool segment::intersects(const reshp::segment& other) const
    {
        int aba = reshp::triangle(start, end, other.start).direction();
        int abb = reshp::triangle(start, end, other.end).direction();
        int baa = reshp::triangle(other.start, other.end, start).direction();
        int bab = reshp::triangle(other.start, other.end, end).direction();
        
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
