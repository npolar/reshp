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
#include <cstdio>

namespace reshp
{
    void handler::subtract(const std::string& basefile, const std::string& maskfile)
    {
        if(verbose_)
            printf("subtract '%s' from '%s'\n", maskfile.c_str(), basefile.c_str());
        
        reshp::shp base, mask;
        
        if(verbose_)
            printf("  loading base shapefile '%s'\n", basefile.c_str());
        
        if(!base.load(basefile))
            return;
        
        if(verbose_)
            printf("  loading mask shapefile '%s'\n", maskfile.c_str());
        
        if(!mask.load(maskfile))
            return;
        
        std::vector<reshp::polygon> basepolys, maskpolys;
        
        if(verbose_)
            printf("  converting base shapefile polygons\n");
        
        for(unsigned r = 0; r < base.records.size(); ++r)
        {
            reshp::shp::record& record = base.records[r];
            
            if(record.type != reshp::shp::shape::polygon)
            {
                printf("    shapefile record type not supported as base shape for subtraction: %s\n", reshp::shp::typestr(record.type));
                continue;
            }
            
            if(!record.polygon)
            {
                printf("    missing polygon data in base shapefile: %s (record #%i)\n", basefile.c_str(), record.number);
                continue;
            }
            
            reshp::polygon poly(*record.polygon);
            basepolys.push_back(poly);
        }
        
        if(verbose_)
            printf("  converting mask shapefile polygons\n");
        
        for(unsigned r = 0; r < mask.records.size(); ++r)
        {
            reshp::shp::record& record = mask.records[r];
            
            if(record.type != reshp::shp::shape::polygon)
            {
                printf("    shapefile record type not supported as mask shape for subtraction: %s\n", reshp::shp::typestr(record.type));
                continue;
            }
            
            if(!record.polygon)
            {
                printf("    missing polygon data in mask shapefile: %s (record #%i)\n", maskfile.c_str(), record.number);
                continue;
            }
            
            reshp::polygon poly(*record.polygon);
            maskpolys.push_back(poly);
        }
        
        if(verbose_)
            printf("  checking polygons for intersections\n");
        
        for(unsigned bpoly = 0; bpoly < basepolys.size(); ++bpoly)
        {
            for(unsigned mpoly = 0; mpoly < maskpolys.size(); ++mpoly)
            {
                if(verbose_)
                    printf("    testing base polygon %05u against mask polygon %05u\n", bpoly + 1, mpoly + 1);
                
                // Check if the mask polygon is entirely inside the base polygon
                if(maskpolys[mpoly].aabb.inside(basepolys[bpoly].aabb) && !maskpolys[mpoly].intersects(basepolys[bpoly]))
                {
                    // Mask polygon is entirely inside base polygon; add mask polygon as inner ring in base polygon
                }
                
                // Otherwise, check if the mask polygon intersects the base polygon
                else if(maskpolys[mpoly].aabb.intersects(basepolys[bpoly].aabb) && maskpolys[mpoly].intersects(basepolys[bpoly]))
                {
                    // Mask polygon intersects base polygon; find intersection points and add to outer ring of base polygon.
                    // Then remove base polygon points between intersections, and add mask polygon points.
                }
            }
        }
    }
}
