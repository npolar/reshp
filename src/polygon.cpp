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
#include <algorithm>

namespace reshp
{
    polygon::intersection::intersection(const reshp::polygon::ring* ring, const reshp::polygon::ring* intersector_ring) : 
        ring(ring),
        intersector(intersector_ring)
    {
    }
    
    polygon::intersection::intersector::intersector(const reshp::polygon::ring* ring) :
        ring(ring)
    {
    }
    
    polygon::ring::ring() :
        type(polygon::ring::outer)
    {
    }
    
    void polygon::ring::calculate_aabb()
    {
        aabb.initialize(); // Initialize AABB with min/max extremes
        
        for(unsigned i = 0; i < points.size(); ++i)
        {
            aabb.min.x = std::min(aabb.min.x, points[i].x);
            aabb.min.y = std::min(aabb.min.y, points[i].y);
            aabb.max.x = std::max(aabb.max.x, points[i].x);
            aabb.max.y = std::max(aabb.max.y, points[i].y);
        }
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
    
    bool polygon::ring::intersects(const reshp::polygon::ring& other, std::vector<reshp::polygon::intersection>* intersections) const
    {
        if(!aabb.intersects(other.aabb) && !other.aabb.intersects(aabb))
            return false;
        
        if(intersections)
            intersections->clear();
        
        reshp::polygon::intersection intersection(this, &other);
        
        for(unsigned tpoint = 0; tpoint < this->points.size(); ++tpoint)
        {
            intersection.segment = reshp::segment(this->points[tpoint], this->points[(tpoint >= this->points.size() - 1) ? 0 : tpoint + 1]);
            
            for(unsigned opoint = 0; opoint < other.points.size(); ++opoint)
            {
                intersection.intersector.segment = reshp::segment(other.points[opoint], other.points[(opoint >= other.points.size() - 1) ? 0 : opoint + 1]);
                
                if(intersection.segment.intersects(intersection.intersector.segment, &intersection.point))
                {
                    if(!intersections)
                        return true;
                    
                    intersections->push_back(intersection);
                }
            }
        }
        
        if(intersections)
            return intersections->size();
            
        return false;
    }
        
    polygon::polygon()
    {
    }
    
    polygon::polygon(const reshp::polygon& poly) :
        aabb(poly.aabb),
        rings(poly.rings)
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
            
            ring.calculate_aabb();
            rings.push_back(ring);
        }
        
        calculate_aabb();
    }
    
    void polygon::calculate_aabb()
    {
        aabb.initialize(); // Initialize AABB with min/max extremes
        
        for(unsigned i = 0; i < rings.size(); ++i)
        {
            aabb.min.x = std::min(aabb.min.x, rings[i].aabb.min.x);
            aabb.min.y = std::min(aabb.min.y, rings[i].aabb.min.y);
            aabb.max.x = std::max(aabb.max.x, rings[i].aabb.max.x);
            aabb.max.y = std::max(aabb.max.y, rings[i].aabb.max.y);
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
    
    bool polygon::intersects(const reshp::polygon& other, std::vector<reshp::polygon::intersection>* intersections) const
    {
        if(!aabb.intersects(other.aabb) && !other.aabb.intersects(aabb))
            return false;
        
        if(intersections)
            intersections->clear();
        
        for(unsigned tring = 0; tring < this->rings.size(); ++tring)
            for(unsigned oring = 0; oring < other.rings.size(); ++oring)
                if(this->rings[tring].intersects(other.rings[oring], intersections))
                    if(!intersections)
                        return true;
        
        if(intersections)
            return intersections->size();
        
        return false;
    }
}
