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

#include "shp.hpp"
#include "file.hpp"

#include <new>
#include <cfloat>
#include <cstdio>
#include <algorithm>

namespace reshp
{
    const char* shp::typestr(const int32_t type)
    {
        switch(type)
        {
        case shp::shape::null:          return "null";
        case shp::shape::point:         return "point";
        case shp::shape::polyline:      return "polyline";
        case shp::shape::polygon:       return "polygon";
        case shp::shape::multipoint:    return "multipoint";
        case shp::shape::zpoint:        return "z-point";
        case shp::shape::zpolyline:     return "z-polyline";
        case shp::shape::zpolygon:      return "z-polygon";
        case shp::shape::zmultipoint:   return "z-multipoint";
        case shp::shape::mpoint:        return "m-point";
        case shp::shape::mpolyline:     return "m-polyline";
        case shp::shape::mpolygon:      return "m-polygon";
        case shp::shape::mmultipoint:   return "m-multipoint";
        case shp::shape::multipatch:    return "multipatch";
        }
        
        return "unknown";
    }
    
    shp::point::point(const double x, const double y) :
        x(x),
        y(y)
    {
    }
    
    shp::polyline::polyline() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
    }
    
    shp::polygon::polygon() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
    }
    
    shp::multipoint::multipoint() :
        num_points(0),
        points(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
    }
    
    shp::zpoint::zpoint(const double x, const double y, const double z, const double m) :
        x(x),
        y(y),
        z(z),
        m(m)
    {
    }
    
    shp::zpolyline::zpolyline() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL),
        z_array(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        z_range[0] = z_range[1] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::zpolygon::zpolygon() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL),
        z_array(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        z_range[0] = z_range[1] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::zmultipoint::zmultipoint() :
        num_points(0),
        points(NULL),
        z_array(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        z_range[0] = z_range[1] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::mpoint::mpoint(const double x, const double y, const double m) :
        x(x),
        y(y),
        m(m)
    {
    }
    
    shp::mpolyline::mpolyline() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::mpolygon::mpolygon() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::mmultipoint::mmultipoint() :
        num_points(0),
        points(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::multipatch::multipatch() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        part_types(NULL),
        points(NULL),
        z_array(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        z_range[0] = z_range[1] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::record::record() :
        number(0),
        length(0),
        type(0),
        shape(NULL),
        point(NULL),
        polyline(NULL),
        polygon(NULL),
        multipoint(NULL),
        zpoint(NULL),
        zpolyline(NULL),
        zpolygon(NULL),
        zmultipoint(NULL),
        mpoint(NULL),
        mpolyline(NULL),
        mpolygon(NULL),
        mmultipoint(NULL),
        multipatch(NULL)
    {
    }

    shp::~shp()
    {
        free();
    }
    
    void shp::free()
    {
        for(unsigned i = 0; i < records.size(); ++i)
            if(records[i].shape)
                delete records[i].shape;
       
        records.clear();
        memset(&header.data, 0, sizeof(header));
    }
    
    bool shp::load(const std::string& filename)
    {
        return false;
    }
    
    bool shp::save(const std::string& filename)
    {
        return false;
    }
}