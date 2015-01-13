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

#ifndef RESHP_AABB_HPP_
#define RESHP_AABB_HPP_

#include "point.hpp"

namespace reshp
{
    struct aabb
    {
        reshp::point min, max;
        
        aabb();
        aabb(const reshp::point& min, const reshp::point& max);
        
        void initialize();
        bool inside(const reshp::aabb&) const;
        bool intersects(const reshp::aabb&) const;
        
        void operator>> (double (&)[4]); // Xmin, Ymin, Xmax, Ymax
        void operator>> (double (&)[6]); // Xmin, Ymin, Zmin, Xmax, Ymax, Zmax
    };
}

#endif // RESHP_AABB_HPP_
