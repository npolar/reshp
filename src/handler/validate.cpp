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
        if(!shp.load(shapefile, false))
        {
            printf("%c\n", (shp.records.size() ? 'E' : 'F'));
            return;
        }
        
        std::vector<reshp::polygon> polys;
        
        for(unsigned i = 0; i < shp.records.size(); ++i)
        {
            if(shp.records[i].polygon)
                polys.push_back(reshp::polygon(*shp.records[i].polygon));
        }
        
        /* TODO:
         * Check for rings without endpoint
         * Check for self-intersection in rings
         * Check for ring-intersection in polygons
         * Check for inner-rings without outer-ring parent
         * Check for correct naming convention
         * Check for required files
         */
        
        printf("D\n");
    } // handler::validate()
} // namespace reshp
