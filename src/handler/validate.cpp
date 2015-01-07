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
#include <cstdio>
#include <vector>

namespace reshp
{
    void handler::validate(const std::string& shapefile)
    {
        /*
        A: Valid ESRI Shapefile, which completely complies with the specifications
        B: Valid ESRI Shapefile, with valid data and clean shapes according to the specifications
        C: Valid ESRI Shapefile, with valid data and no self-intersecting shapes
        D: Valid ESRI Shapefile, with no broken records or missing data
        E: Valid ESRI Shapefile, with broken records or missing data
        F: Not a valid ESRI Shapefile
        */
        
        reshp::shp shp;
        if(!shp.load(shapefile, verbose_))
        {
            printf("%c\n", (shp.records.size() ? 'E' : 'F'));
            return;
        }
        
        char grade = 'A';
        std::vector<reshp::polygon> polys;
        
        for(unsigned i = 0; i < shp.records.size(); ++i)
        {
            if(shp.records[i].polygon)
            {
                reshp::polygon poly(*shp.records[i].polygon);
                polys.push_back(poly);
                
                // Check for correct amount of rings
                if(poly.rings.size() != static_cast<unsigned>(shp.records[i].polygon->num_parts))
                {
                    if(verbose_)
                        printf("missing ring endpoints in polygon %lu\n", polys.size() - 1);
                    
                    if(grade < 'D')
                        grade = 'D';
                }
            }
        }
        
        for(unsigned p = 0; p < polys.size(); ++p)
        {
            // Check for self-intersections
            if(polys[p].intersects())
            {
                if(verbose_)
                    printf("self-intersections in polygon %u\n", p);
                
                if(grade < 'D')
                    grade = 'D';
            }
            
            unsigned outer_rings = 0;
            
            for(unsigned r = 0; r < polys[p].rings.size(); ++r)
                if(polys[p].rings[r].type == reshp::polygon::ring::outer)
                    outer_rings++;
            
            // Check for missing outer rings
            if(polys[p].rings.size() && !outer_rings)
            {
                if(verbose_)
                    printf("missing outer rings in polygon %u\n", p);
                
                if(grade < 'C')
                    grade = 'C';
            }
        }
        
        /* TODO:
         * Check for inner-rings without outer-ring parent
         * Check for correct naming convention
         * Check for required files
         */
        
        if(grade < 'B')
            grade = 'B';
        
        printf("%c\n", grade);
    } // handler::validate()
} // namespace reshp
