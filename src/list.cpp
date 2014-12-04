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
                int32_t rpoints[rings];
                
                for(int32_t r = 0; r < rings; ++r)
                    rpoints[r] = shp.records[i].polygon->parts[r];
                
                for(int32_t p = 0; p < points; ++p)
                {
                    for(int32_t r = 0; r < rings; ++r)
                        if(rpoints[r] == p)
                            printf("    ring %i:\n", r);
                    
                    printf("      [%.4f, %.4f]\n", shp.records[i].polygon->points[p].x,  shp.records[i].polygon->points[p].y);
                }
            }
        }
    }
}
