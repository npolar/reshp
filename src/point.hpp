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

#ifndef RESHP_POINT_HPP_
#define RESHP_POINT_HPP_

namespace reshp
{
    struct segment;
    
    struct point
    {
        double x, y, z;
        
        point(const double x = 0.0, const double y = 0.0, const double z = 0.0);
        
        bool intersects(const reshp::segment&) const;
    };
}

#endif // RESHP_POINT_HPP_