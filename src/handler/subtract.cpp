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
                        else if(verbose_)
                            printf("      ignored inner ring #%u in mask polygon\n", r);
                    }
                    
                    continue;
                }
                
                std::vector<reshp::polygon::intersection> intersections;
                std::vector<reshp::polygon::ring> rings;
                
                // Mask polygon intersects base polygon
                if(maskpolys[mpoly].intersects(basepolys[bpoly], &intersections))
                {
                    if(verbose_)
                        printf("    %lu intersections found between base polygon #%u and mask polygon #%u:\n", intersections.size(), bpoly, mpoly);
                        
                    for(unsigned i = 0; i < intersections.size(); ++i)
                    {
                        if(verbose_)
                            printf("      [%8.4f, %8.4f]\n", intersections[i].point.x, intersections[i].point.y);
                        
                        if(basepolys[bpoly].contains(intersections[i].segment->start))
                        {
                            bool done = false;
                            reshp::polygon::ring ring; // outer by default
                            
                            for(unsigned n = 0, s = intersections[i].segment_index; !done && n < intersections[i].ring->segments.size(); ++n)
                            {
                                int tindex = -1;
                                reshp::point intersection;
                                reshp::segment segment((n ? intersections[i].ring->segments[s].end : intersections[i].point), intersections[i].ring->segments[s].start);
                                
                                for(unsigned t = 0; n && t < intersections[i].intersector.ring->segments.size(); ++t)
                                {
                                    if(segment.intersects(intersections[i].intersector.ring->segments[t], &intersection))
                                    {
                                        segment.end = intersection;
                                        tindex = t;
                                        break;
                                    }
                                }
                                
                                ring.segments.push_back(segment);
                                
                                if(tindex >= 0)
                                {
                                    for(unsigned o = 0, t = tindex; !done && o < intersections[i].intersector.ring->segments.size(); ++o)
                                    {
                                        segment = reshp::segment((o ? intersections[i].intersector.ring->segments[t].start : intersection), intersections[i].intersector.ring->segments[t].end);
                                        
                                        if(t == static_cast<unsigned>(intersections[i].intersector.segment_index))
                                        {
                                            segment.end = intersections[i].point;
                                            done = true;
                                        }
                                        
                                        ring.segments.push_back(segment);
                                        
                                        t = (t < intersections[i].intersector.ring->segments.size() - 1 ? t + 1 : 0);
                                    }
                                }
                                
                                s = (s ? s - 1 : intersections[i].ring->segments.size() - 1);
                            }
                            
                            rings.push_back(ring);
                        }
                    }
                    
                    basepolys[bpoly].rings = rings;
                    basepolys[bpoly].calculate_aabb();
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
