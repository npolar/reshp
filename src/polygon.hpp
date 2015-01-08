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

#ifndef RESHP_POLYGON_HPP_
#define RESHP_POLYGON_HPP_

#include "point.hpp"
#include "aabb.hpp"
#include "shp.hpp"
#include "segment.hpp"
#include <vector>

namespace reshp
{
    struct polygon
    {
        struct ring;
        
        struct intersection
        {
            reshp::point point;
            reshp::polygon::ring* ring;
            reshp::segment* segment;
            
            struct intersector
            {
                reshp::polygon::ring* ring;
                reshp::segment* segment;
                
                intersector(reshp::polygon::ring* = NULL);
            } intersector;
            
            intersection(reshp::polygon::ring* = NULL, reshp::polygon::ring* intersector_ring = NULL);
        };
        
        struct ring
        {
            ring();
            
            reshp::aabb aabb;
            enum { outer, inner } type;
            std::vector<reshp::segment> segments;
            
            void calculate_aabb();
            bool contains(const reshp::point&) const;
            bool inside(const reshp::polygon::ring&) const;
            bool intersects() const; // Self-intersection
            bool intersects(const reshp::polygon::ring&, std::vector<reshp::polygon::intersection>* intersections = NULL) const;
        };
        
        polygon();
        polygon(const reshp::polygon&);
        polygon(const reshp::shp::polygon&);
        
        reshp::aabb aabb;
        std::vector<reshp::polygon::ring> rings;
        
        void calculate_aabb();
        bool contains(const reshp::point&) const;
        bool inside(const reshp::polygon&) const;
        bool intersects() const; // Self-intersection
        bool intersects(const reshp::polygon&, std::vector<reshp::polygon::intersection>* intersections = NULL) const;
        
        void operator>> (reshp::shp::polygon&) const;
    };
}

#endif // RESHP_POLYGON_HPP_
