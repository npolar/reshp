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

#include "../handler.hpp"
#include "../polygon.hpp"
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
            
            if(shp.records[i].polygon)
            {
                reshp::polygon poly(*shp.records[i].polygon);
                
                unsigned rings = poly.rings.size();
                unsigned points = 0;
                
                for(unsigned r = 0; r < rings; ++r)
                    points += poly.rings[r].points.size();
                
                printf(":%*s%5i point%c", int(13 - strlen(type)), " ", points, (points == 1 ? ' ' : 's'));
                printf(", %5i ring%c", rings, (rings == 1 ? ' ' : 's'));
                printf("\n");
                
                if(full)
                {
                    for(unsigned r = 0; r < poly.rings.size(); ++r)
                    {
                        printf("    ring %i (%s, %lu points):\n", r, (poly.rings[r].type == reshp::polygon::ring::inner ? "inner" : "outer"), poly.rings[r].points.size());
                        
                        for(unsigned p = 0; p < poly.rings[r].points.size(); ++p)
                        {
                            printf("      [%.4f, %.4f]\n", poly.rings[r].points[p].x, poly.rings[r].points[p].y);
                        }
                    }
                }
            }
            else printf("\n");
        } // records
    } // handler::list()
} // namespace reshp
