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
#include "../shx.hpp"

#include <vector>
#include <cstdio>

namespace reshp
{
    void handler::subtract(const std::string& basefile, const std::string& maskfile, const char* outputfile)
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
            printf("  testing collision in %lu polygons\n", basepolys.size() * maskpolys.size());
        
        for(unsigned bpoly = 0; bpoly < basepolys.size(); ++bpoly)
        {
            for(unsigned mpoly = 0; mpoly < maskpolys.size(); ++mpoly)
            {
                // Mask polygon is entirely inside base polygon
                if(maskpolys[mpoly].inside(basepolys[bpoly]))
                {
                    if(verbose_)
                        printf("    mask polygon #%u is entirely inside base polygon #%u\n", mpoly, bpoly);
                    
                    for(unsigned r = 0; r < maskpolys[mpoly].rings.size(); ++ r)
                    {
                        if(maskpolys[mpoly].rings[r].type == reshp::polygon::ring::outer)
                        {
                            if(verbose_)
                                printf("      outer ring #%u added to base polygon as inner ring\n", r);
                            
                            basepolys[bpoly].rings.push_back(maskpolys[mpoly].rings[r]);
                            basepolys[bpoly].rings.back().type = reshp::polygon::ring::inner;
                        }
                        else printf("      ignored inner ring #%u in mask polygon\n", r);
                    }
                    
                    continue;
                }
                
                std::vector<reshp::polygon::intersection> intersections;
                
                // Mask polygon intersects base polygon
                if(maskpolys[mpoly].intersects(basepolys[bpoly], &intersections))
                {
                    if(verbose_)
                        printf("    %lu intersections found between base polygon #%u and mask polygon #%u:\n", intersections.size(), bpoly, mpoly);
                        
                    for(unsigned i = 0; i < intersections.size(); ++i)
                    {
                        if(verbose_)
                            printf("      %f, %f\n", intersections[i].point.x, intersections[i].point.y);
                        
                        /* TODO:
                        // Add intersection point as segment start/end point of intersector
                        if(maskpolys[mpoly].contains(intersections[i].intersector.segment->start))
                            intersections[i].intersector.segment->start = intersections[i].point;
                        else if(maskpolys[mpoly].contains(intersections[i].intersector.segment->end))
                            intersections[i].intersector.segment->end = intersections[i].point;
                        */
                        
                        /* TODO:
                        // Add segments between intersector points of mask polygon to base polygon intersector ring
                        if(basepolys[bpoly].contains(intersections[i].segment->start))
                        {
                            intersections[i].intersector.ring->segments.insert(
                                intersections[i].intersector.ring->segments.begin() + intersections[i].intersector.segment_index + 1,
                                reshp::segment(intersections[i].point, intersections[i].segment->start)
                            );
                        }
                        else if(maskpolys[mpoly].contains(intersections[i].segment->end))
                        {
                            intersections[i].intersector.ring->segments.insert(
                                intersections[i].intersector.ring->segments.begin() + intersections[i].intersector.segment_index + 1,
                                reshp::segment(intersections[i].point, intersections[i].segment->end)
                            );
                        }
                        */
                    }
                }
            } // maskpolys
        } // basepolys
        
        if(outputfile)
        {
            reshp::shp output;
            
            for(unsigned p = 0; p < basepolys.size(); ++p)
            {
                reshp::shp::record record;
                
                if((record.polygon = new (std::nothrow) reshp::shp::polygon()))
                {
                    record.shape = record.polygon; // Automatically deallocates record.polygon on output dtor
                    record.type = reshp::shp::shape::polygon;
                    
                    basepolys[p].calculate_aabb();
                    basepolys[p] >> *record.polygon;
                    basepolys[p].aabb >> output.header.box;
                    output.header.type = reshp::shp::shape::polygon;
                    
                    output.records.push_back(record);
                }
            }
            
            if(output.save(outputfile, verbose_))
            {
                reshp::shx indexfile;
                
                if(indexfile.load(output, verbose_))
                {
                    std::string indexfilename(outputfile);
                    
                    if(indexfilename.substr(indexfilename.length() - 4) == ".shp")
                        indexfilename.replace(indexfilename.length() - 4, 4, ".shx");
                    else indexfilename += ".shx";
                    
                    indexfile.save(indexfilename, verbose_);
                }
            }
        }
    } // handler::subtract()
} // namespace reshp
