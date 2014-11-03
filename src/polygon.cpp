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

#include "polygon.hpp"
#include "segment.hpp"
#include <algorithm>

namespace reshp
{
    polygon::ring::ring() :
        type(polygon::ring::outer)
    {
    }
    
    bool polygon::ring::contains(const reshp::point& point) const
    {
        bool inside = false;
        
        for(unsigned i = 0, j = points.size() - 1; i < points.size(); j = i++)
        {
            if(((points[i].y > point.y) != (points[j].y > point.y))
            && (point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x))
                inside = !inside;
        }
        
        return inside;
    }
    
    bool polygon::ring::inside(const reshp::polygon::ring& other) const
    {
        if(!aabb.inside(other.aabb))
            return false;
        
        for(unsigned i = 0; i < points.size(); ++i)
            if(!other.contains(points[i]))
                return false;
        
        return true;
    }
    
    bool polygon::ring::intersects(const reshp::polygon::ring& other) const
    {
        if(!aabb.intersects(other.aabb))
            return false;
        
        for(unsigned tpoint = 0; tpoint < this->points.size(); ++tpoint)
        {
            reshp::segment tline(this->points[tpoint], this->points[(tpoint >= this->points.size() - 1) ? 0 : tpoint + 1]);
            
            for(unsigned opoint = 0; opoint < other.points.size(); ++opoint)
            {
                reshp::segment oline(other.points[opoint], other.points[(opoint >= other.points.size() - 1) ? 0 : opoint + 1]);
                
                if(tline.intersects(oline))
                    return true;
            }
        }
            
        return false;
    }
        
    polygon::polygon()
    {
    }
    
    polygon::polygon(const reshp::shp::polygon& poly)
    {
        for(int32_t part = 0; part < poly.num_parts; ++part)
        {
            reshp::polygon::ring ring;
            int32_t first = poly.parts[part];
            
            for(int32_t p = first; p < poly.num_points; ++p)
            {
                reshp::point point(poly.points[p].x, poly.points[p].y);
                
                if((p != first)
                && (point.x == poly.points[first].x)
                && (point.y == poly.points[first].y))
                    break;
                
                // Recalculate ring bounding box
                ring.aabb.min.x = std::min(ring.aabb.min.x, point.x);
                ring.aabb.min.y = std::min(ring.aabb.min.y, point.y);
                ring.aabb.max.x = std::max(ring.aabb.max.x, point.x);
                ring.aabb.max.y = std::max(ring.aabb.max.y, point.y);
                
                // Recalculate polygon bounding box
                aabb.min.x = std::min(aabb.min.x, ring.aabb.min.x);
                aabb.min.y = std::min(aabb.min.y, ring.aabb.min.y);
                aabb.max.x = std::max(aabb.max.x, ring.aabb.max.x);
                aabb.max.y = std::max(aabb.max.y, ring.aabb.max.y);
                    
                ring.points.push_back(point);
            }
            
            if(ring.points.size() >= 3)
            {
                // 0: Colinear, >0: Clockwise, <0: Counter-clockwise
                int dir = (((ring.points[1].y - ring.points[0].y) * (ring.points[2].x - ring.points[1].x)) - 
                           ((ring.points[1].x - ring.points[0].x) * (ring.points[2].y - ring.points[1].y)));
                
                // Clockwise: Outer, Counter-clockwise: Inner
                ring.type = (dir > 0 ? ring::outer : ring::inner);
            }
            
            rings.push_back(ring);
        }
    }
    
    bool polygon::inside(const reshp::polygon& other) const
    {
        if(!aabb.inside(other.aabb))
            return false;
        
        for(unsigned tring = 0; tring < this->rings.size(); ++tring)
            for(unsigned oring = 0; oring < other.rings.size(); ++oring)
                if(!this->rings[tring].inside(other.rings[oring]))
                    return false;
        
        return true;
    }
    
    bool polygon::intersects(const reshp::polygon& other) const
    {
        if(!aabb.intersects(other.aabb))
            return false;
        
        for(unsigned tring = 0; tring < this->rings.size(); ++tring)
            for(unsigned oring = 0; oring < other.rings.size(); ++oring)
                if(this->rings[tring].intersects(other.rings[oring]))
                    return true;
        
        return false;
    }
}
