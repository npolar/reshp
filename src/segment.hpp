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

#ifndef RESHP_SEGMENT_HPP_
#define RESHP_SEGMENT_HPP_

#include "point.hpp"

namespace reshp
{
    struct segment
    {
        reshp::point start, end;
        
        segment(const reshp::point& start, const reshp::point& end);
        
        bool intersects(const reshp::segment&) const;
    };
}

#endif // RESHP_SEGMENT_HPP_
