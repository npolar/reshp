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
                    points += poly.rings[r].segments.size();
                
                if(full)
                {
                    printf(": %i point%c", points, (points == 1 ? ' ' : 's'));
                    printf(", %i ring%c\n", rings, (rings == 1 ? ' ' : 's'));
                    
                    for(unsigned r = 0; r < poly.rings.size(); ++r)
                    {
                        printf("    ring %i (%s, %lu segments):\n", r, (poly.rings[r].type == reshp::polygon::ring::inner ? "inner" : "outer"), poly.rings[r].segments.size());
                        
                        for(unsigned s = 0; s < poly.rings[r].segments.size(); ++s)
                        {
                            printf("      [%8.4f, %8.4f] -> [%8.4f, %8.4f]\n", poly.rings[r].segments[s].start.x, poly.rings[r].segments[s].start.y, poly.rings[r].segments[s].end.x, poly.rings[r].segments[s].end.y);
                        }
                    }
                }
                else
                {
                    printf(":%*s%5i point%c", int(13 - strlen(type)), " ", points, (points == 1 ? ' ' : 's'));
                    printf(", %5i ring%c\n", rings, (rings == 1 ? ' ' : 's'));
                }
            }
            else printf("\n");
        } // records
    } // handler::list()
} // namespace reshp
