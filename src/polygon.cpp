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
#include <new>

namespace reshp
{
    polygon::intersection::intersection(reshp::polygon::ring* ring, reshp::polygon::ring* intersector_ring) : 
        ring(ring),
        segment(NULL),
        segment_index(-1),
        intersector(intersector_ring)
    {
    }
    
    polygon::intersection::intersector::intersector(reshp::polygon::ring* ring) :
        ring(ring),
        segment(NULL),
        segment_index(-1)
    {
    }
    
    polygon::ring::ring() :
        type(polygon::ring::outer)
    {
    }
    
    void polygon::ring::calculate_aabb()
    {
        aabb.initialize(); // Initialize AABB with min/max extremes
        
        for(unsigned i = 0; i < segments.size(); ++i)
        {
            aabb.min.x = std::min(aabb.min.x, segments[i].start.x);
            aabb.min.y = std::min(aabb.min.y, segments[i].start.y);
            aabb.max.x = std::max(aabb.max.x, segments[i].start.x);
            aabb.max.y = std::max(aabb.max.y, segments[i].start.y);
        }
    }
    
    bool polygon::ring::contains(const reshp::point& point) const
    {
        bool inside = false;
        
        for(unsigned i = 0, j = segments.size() - 1; i < segments.size(); j = i++)
        {
            if(((segments[i].start.y > point.y) != (segments[j].start.y > point.y))
            && (point.x < (segments[j].start.x - segments[i].start.x) * (point.y - segments[i].start.y) / (segments[j].start.y - segments[i].start.y) + segments[i].start.x))
                inside = !inside;
        }
        
        return inside;
    }
    
    bool polygon::ring::inside(const reshp::polygon::ring& other) const
    {
        if(!aabb.inside(other.aabb))
            return false;
        
        for(unsigned i = 0; i < segments.size(); ++i)
            if(!other.contains(segments[i].start))
                return false;
            
        return true;
    }
    
    bool polygon::ring::intersects() const
    {
        // TODO: Efficiency test
        
        for(unsigned i = 0, s = segments.size(); i < s - 2; ++i)
            for(unsigned j = i + 1; j < segments.size(); ++j)
                if(j - i > 1 && j < (s - 1) + i)
                    if(segments[i].intersects(segments[j]))
                        return true;
                    
        return false;
    }
    
    bool polygon::ring::intersects(const reshp::polygon::ring& other, std::vector<reshp::polygon::intersection>* intersections) const
    {
        if(!aabb.intersects(other.aabb) && !other.aabb.intersects(aabb))
            return false;
        
        if(intersections)
            intersections->clear();
        
        reshp::polygon::ring* mutable_this = const_cast<reshp::polygon::ring*>(this);
        reshp::polygon::ring* mutable_other = const_cast<reshp::polygon::ring*>(&other);
        reshp::polygon::intersection intersection(mutable_this, mutable_other);
        
        for(unsigned tseg = 0; tseg < this->segments.size(); ++tseg)
        {
            intersection.segment = &mutable_this->segments[tseg];
            intersection.segment_index = tseg;
            
            for(unsigned oseg = 0; oseg < other.segments.size(); ++oseg)
            {
                intersection.intersector.segment = &mutable_other->segments[oseg];
                intersection.intersector.segment_index = oseg;
                
                if(intersection.segment->intersects(*intersection.intersector.segment, &intersection.point))
                {
                    if(!intersections)
                        return true;
                    
                    intersections->push_back(intersection);
                }
            }
        }
        
        if(intersections && intersections->size())
            return true;
            
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
            int32_t first_index = poly.parts[part];
            reshp::point first(poly.points[first_index].x, poly.points[first_index].y);
            
            for(int32_t p = first_index + 1; p < poly.num_points; ++p)
            {
                reshp::segment segment(reshp::point(poly.points[p - 1].x, poly.points[p - 1].y),
                                       reshp::point(poly.points[p].x, poly.points[p].y));
                
                ring.segments.push_back(segment);
                
                if(segment.end == first)
                    break;
            }
            
            if(ring.segments.size() >= 3)
            {
                int dir = (((ring.segments[0].end.y - ring.segments[0].start.y) * (ring.segments[1].end.x - ring.segments[1].start.x)) -
                           ((ring.segments[0].end.x - ring.segments[0].start.x) * (ring.segments[1].end.y - ring.segments[1].start.y)));
                
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
    
    bool polygon::contains(const reshp::point& point) const
    {
        bool inside = false;
        
        // Return false if point is inside inner ring (hole)
        for(unsigned r = 0; r < rings.size(); ++r)
        {
            if(rings[r].contains(point))
            {
                if(rings[r].type == reshp::polygon::ring::inner)
                {
                    return false;
                }
                else if(!inside)
                    inside = true;
            }
        }
        
        return inside;
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
    
    bool polygon::intersects() const
    {
        for(unsigned i = 0; i < rings.size(); ++i)
        {
            if(rings[i].intersects())
                return true;
                
            for(unsigned j = i + 1; j < rings.size(); ++j)
                if(rings[i].intersects(rings[j]))
                    return true;
        }
        
        return false;
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
    
    void polygon::operator>> (reshp::shp::polygon& poly) const
    {
        poly.box[0] = aabb.min.x;
        poly.box[1] = aabb.min.y;
        poly.box[2] = aabb.max.x;
        poly.box[3] = aabb.max.y;
        
        unsigned points = 0, parts = rings.size();
        
        for(unsigned p = 0; p < parts; ++p)
            points += rings[p].segments.size() + 1;
        
        if(poly.parts)
            delete poly.parts;
        
        if(poly.points)
            delete poly.points;
        
        if((poly.parts = new (std::nothrow) int32_t[parts]))
        {
            if((poly.points = new (std::nothrow) reshp::shp::point[points]))
            {
                for(unsigned r = 0; r < parts; ++r)
                {
                    poly.parts[poly.num_parts++] = poly.num_points;
                    
                    for(unsigned s = 0; s < rings[r].segments.size(); ++s)
                        poly.points[poly.num_points++] = reshp::shp::point(rings[r].segments[s].start.x, rings[r].segments[s].start.y);
                    
                    poly.points[poly.num_points++] = reshp::shp::point(rings[r].segments[0].start.x, rings[r].segments[0].start.y);
                }
            }
        }
    }
}
