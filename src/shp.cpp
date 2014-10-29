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
    
    shp::polyline::~polyline()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
    }
    
    shp::polygon::polygon() :
        num_parts(0),
        num_points(0),
        parts(NULL),
        points(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
    }
    
    shp::polygon::~polygon()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
    }
    
    shp::multipoint::multipoint() :
        num_points(0),
        points(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
    }
    
    shp::multipoint::~multipoint()
    {
        if(points)  delete[] points;
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
    
    shp::zpolyline::~zpolyline()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
        if(z_array) delete[] z_array;
        if(m_array) delete[] m_array;
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
    
    shp::zpolygon::~zpolygon()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
        if(z_array) delete[] z_array;
        if(m_array) delete[] m_array;
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
    
    shp::zmultipoint::~zmultipoint()
    {
        if(points)  delete[] points;
        if(z_array) delete[] z_array;
        if(m_array) delete[] m_array;
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
    
    shp::mpolyline::~mpolyline()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
        if(m_array) delete[] m_array;
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
    
    shp::mpolygon::~mpolygon()
    {
        if(parts)   delete[] parts;
        if(points)  delete[] points;
        if(m_array) delete[] m_array;
    }
    
    shp::mmultipoint::mmultipoint() :
        num_points(0),
        points(NULL),
        m_array(NULL)
    {
        box[0] = box[1] = box[2] = box[3] = 0.0;
        m_range[0] = m_range[1] = 0.0;
    }
    
    shp::mmultipoint::~mmultipoint()
    {
        if(points)  delete[] points;
        if(m_array) delete[] m_array;
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
    
    shp::multipatch::~multipatch()
    {
        if(parts)       delete[] parts;
        if(part_types)  delete[] part_types;
        if(points)      delete[] points;
        if(z_array)     delete[] z_array;
        if(m_array)     delete[] m_array;
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
        memset(&header, 0, sizeof(header));
    }
    
    bool shp::load(const std::string& filename)
    {
        free();
        reshp::file file;
        
        if(file.open(filename, reshp::file::mode::read | reshp::file::mode::binary))
        {
            // Read shapefile header
            if(!file.geti(header.identifier, endian::big)
            || !file.read(reinterpret_cast<char*>(header.unused), sizeof(header.unused))
            || !file.geti(header.length, endian::big)
            || !file.geti(header.version, endian::little)
            || !file.geti(header.type, endian::little)
            || !file.getf(header.box[0], endian::little)
            || !file.getf(header.box[1], endian::little)
            || !file.getf(header.box[2], endian::little)
            || !file.getf(header.box[3], endian::little)
            || !file.getf(header.z_range[0], endian::little)
            || !file.getf(header.z_range[1], endian::little)
            || !file.getf(header.z_range[0], endian::little)
            || !file.getf(header.m_range[1], endian::little))
            {
                fprintf(stderr, "could not read shapefile header from: %s\n", filename.c_str());
                return false;
            }
            
            // Verify shapefile identifier
            if(header.identifier != shp::IDENTIFIER)
            {
                fprintf(stderr, "file is not a valid shapefile: %s\n", filename.c_str());
                return false;
            }
            
            // Verify filesize with length property
            if(header.length != static_cast<int32_t>(file.size() / 2))
            {
                fprintf(stderr, "shapesize length mismatch in: %s\n", filename.c_str());
                return false;
            }
            
            // Verify shapefile version
            if(header.version != shp::VERSION)
            {
                fprintf(stderr, "unsupported shapefile version in: %s (%i)\n", filename.c_str(), header.version);
                return false;
            }
            
            // TODO: Read shapefile records
            
            file.close();
            return true;
        }
        
        fprintf(stderr, "could not open file for reading: %s\n", filename.c_str());
        return false;
    }
    
    bool shp::save(const std::string& filename)
    {
        reshp::file file;
        
        if(!file.open(filename, reshp::file::mode::write | reshp::file::mode::binary))
        {
            // TODO: Write shapefile header
            // TODO: Write shapefiel records
            
            file.close();
            return true;
        }
        
        fprintf(stderr, "could not open file for writing: %s\n", filename.c_str());
        return false;
    }
}