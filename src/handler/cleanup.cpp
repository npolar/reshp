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
#include <cstdio>
#include <vector>

namespace reshp
{
    void handler::cleanup(const std::string& shapefile, const char* outputfile)
    {
        if(verbose_)
            printf("shapefile cleanup\n");
        
        reshp::shp shp;
        
        if(verbose_)
            printf("  loading shapefile '%s'\n", shapefile.c_str());
        
        if(!shp.load(shapefile, verbose_))
            return;
        
        std::vector<reshp::polygon> polys;
        
        for(unsigned i = 0; i < shp.records.size(); ++i)
        {
            if(shp.records[i].polygon)
            {
                reshp::polygon poly(*shp.records[i].polygon);
                unsigned index = polys.size(), outer_rings = 0;
                
                /*
                // Remove self-intersecting polygons
                if(poly.intersects())
                {
                    if(verbose_)
                        printf("    removed self-intersecting polygon (record #%u)\n", i);
                    
                    continue;
                }
                */
                
                // Add missing segments for polygon rings
                for(unsigned r = 0; r < poly.rings.size(); ++r)
                {
                    if(poly.rings[r].type == reshp::polygon::ring::outer)
                        outer_rings++;
                    
                    if(poly.rings[r].segments.front().start != poly.rings[r].segments.back().end)
                    {
                        if(verbose_)
                            printf("    added missing ring segment in polygon %u, ring %u\n", index, r);
                        
                        poly.rings[r].segments.push_back(reshp::segment(poly.rings[r].segments.back().end, poly.rings[r].segments.front().start));
                    }
                    
                    /* TODO:
                     * Remove self-intersections
                     * Remove inner rings intersecting outer rings
                     * Remove shapes with unfixable missing data
                     * Recalculate ring-directions for multi-ringed shapes
                     */
                }
                
                for(unsigned r = 0; r < poly.rings.size(); ++r)
                {
                    // Invert inner-rings not contained within an outer-ring
                    if(poly.rings[r].type == reshp::polygon::ring::inner)
                    {
                        bool inside = false;
                        
                        for(unsigned o = 0; o < poly.rings.size(); ++o)
                        {
                            if(o != r && poly.rings[r].inside(poly.rings[o]))
                            {
                                inside = true;
                                break;
                            }
                        }
                        
                        if(!inside)
                        {
                            if(verbose_)
                                printf("    inverting inner-ring %u to outer-ring in polygon %u\n", r, index);
                            
                            poly.rings[r].invert();
                        }
                    }
                }
                
                // Recalculate polygon bounding boxes
                if(verbose_)
                    printf("    recalculating bounding boxes for polygon %u\n", index);
                
                poly.calculate_aabb();
                polys.push_back(poly);
            }
        }
        
        // TODO: Recalculate file header data

        if(outputfile)
        {
            reshp::aabb aabb;
            reshp::shp output;
            
            for(unsigned p = 0; p < polys.size(); ++p)
            {
                reshp::shp::record record;
                
                if((record.polygon = new (std::nothrow) reshp::shp::polygon()))
                {
                    record.shape = record.polygon; // Automatically deallocates record.polygon on output dtor
                    record.type = reshp::shp::shape::polygon;
                    
                    polys[p] >> *record.polygon;
                    
                    aabb.min.x = std::min(aabb.min.x, polys[p].aabb.min.x);
                    aabb.min.y = std::min(aabb.min.y, polys[p].aabb.min.y);
                    aabb.max.x = std::max(aabb.max.x, polys[p].aabb.max.x);
                    aabb.max.y = std::max(aabb.max.y, polys[p].aabb.max.y);
                    
                    output.records.push_back(record);
                }
            }
            
            aabb >> output.header.box;
            output.header.type = reshp::shp::shape::polygon;
            
            if(output.save(outputfile, verbose_))
            {
                if(verbose_)
                    printf("  shapefile successfully written to: %s\n", outputfile);
                
                reshp::shx indexfile;
                
                if(indexfile.load(output, verbose_))
                {
                    std::string indexfilename(outputfile);
                    
                    if(indexfilename.substr(indexfilename.length() - 4) == ".shp")
                        indexfilename.replace(indexfilename.length() - 4, 4, ".shx");
                    else indexfilename += ".shx";
                    
                    if(indexfile.save(indexfilename, verbose_) && verbose_)
                        printf("  shapefile index successfully written to: %s\n", indexfilename.c_str());
                }
            }
        }
    } // handler::grade()
} // namespace reshp
