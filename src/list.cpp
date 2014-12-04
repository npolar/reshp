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

#include "handler.hpp"
#include "polygon.hpp"
#include <vector>

namespace reshp
{
    void handler::list(const std::string& shapefile, const bool full)
    {
        reshp::shp shp;
        if(!shp.load(shapefile))
            return;
        
        printf("Filename:   %s\n", shapefile.c_str());
        printf("Version:    %i\n", shp.header.version);
        printf("Type:       %s\n", shp::typestr(shp.header.type));
        printf("Bounding:   [%.4f, %.4f] [%.4f, %.4f]\n", shp.header.box[0], shp.header.box[1], shp.header.box[2], shp.header.box[3]);
        printf("Records:    %lu\n\n", shp.records.size());
        
        for(unsigned i = 0; i < shp.records.size(); ++i)
        {
            const char* type = shp::typestr(shp.records[i].type);
            
            printf("  record %05i (%s)", shp.records[i].number, type);
            
            int32_t points = 0, rings = 0;
            
            if(shp.records[i].polygon)
            {
                rings = shp.records[i].polygon->num_parts;
                points = shp.records[i].polygon->num_points;
            }
            
            if(points)  printf(":%*s%5i point%c", int(13 - strlen(type)), " ", points, (points == 1 ? ' ' : 's'));
            if(rings)   printf(", %5i ring%c", rings, (rings == 1 ? ' ' : 's'));
            
            printf("\n");
            
            if(full)
            {
                if(shp.records[i].polygon)
                {
                    reshp::polygon poly(*shp.records[i].polygon);
                    
                    for(unsigned r = 0; r < poly.rings.size(); ++r)
                    {
                        printf("    ring %i (%s):\n", r, (poly.rings[r].type == reshp::polygon::ring::inner ? "inner" : "outer"));
                        
                        for(unsigned p = 0; p < poly.rings[r].points.size(); ++p)
                        {
                            printf("      [%.4f, %.4f]\n", poly.rings[r].points[p].x, poly.rings[r].points[p].y);
                        }
                    }
                }
            }
        } // records
    } // handler::list()
} // namespace reshp
