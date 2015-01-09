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
#include <cstdio>
#include <cstring>

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
    
    shp::shape::~shape()
    {
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
    
    shp::shp()
    {
        header.identifier = shp::IDENTIFIER;
        memset(header.unused, 0, sizeof(header.unused));
        header.length = 50;
        header.version = shp::VERSION;
        header.type = shp::shape::null;
        header.box[0] = header.box[1] = header.box[2] = header.box[3] = 0.0;
        header.z_range[0] = header.z_range[1] = 0.0;
        header.m_range[0] = header.m_range[1] = 0.0;
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
    
    bool shp::load(const std::string& filename, const bool errorlog)
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
                if(errorlog) fprintf(stderr, "could not read shapefile header from: %s\n", filename.c_str());
                return false;
            }
            
            // Verify shapefile identifier
            if(header.identifier != shp::IDENTIFIER)
            {
                if(errorlog) fprintf(stderr, "file is not a valid shapefile: %s\n", filename.c_str());
                return false;
            }
            
            // Verify filesize with length property
            if(header.length != static_cast<int32_t>(file.size() / 2))
            {
                if(errorlog) fprintf(stderr, "shapesize length mismatch in: %s\n", filename.c_str());
                return false;
            }
            
            // Verify shapefile version
            if(header.version != shp::VERSION)
            {
                if(errorlog) fprintf(stderr, "unsupported shapefile version in: %s (%i)\n", filename.c_str(), header.version);
                return false;
            }
            
            // Read shapefile records
            for(unsigned index = 0; !file.eof(); ++index)
            {
                shp::record record;
                
                if(!file.geti(record.number, endian::big)
                || !file.geti(record.length, endian::big)
                || !file.geti(record.type, endian::little)) // Actually not a part of the record header per se
                {
                    if(errorlog) fprintf(stderr, "could not read record header from shapefile: %s (index %i)\n", filename.c_str(), index);
                    return false;
                }
                
                if(record.type == shp::shape::null)
                {
                    // Empty shape (no additional data)
                }
                else if(record.type == shp::shape::point)
                {
                    if(!(record.point = new (std::nothrow) shp::point))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for point record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    
                    if(!file.getf(record.point->x, endian::little)
                    || !file.getf(record.point->y, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "missing data for point record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.point;
                        return false;
                    }
                    
                    record.shape = record.point;
                }
                else if(record.type == shp::shape::polyline)
                {
                    if(!(record.polyline = new (std::nothrow) shp::polyline))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polyline record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    
                    if(!file.getf(record.polyline->box[0], endian::little) // Xmin
                    || !file.getf(record.polyline->box[1], endian::little) // Ymin
                    || !file.getf(record.polyline->box[2], endian::little) // Xmax
                    || !file.getf(record.polyline->box[3], endian::little) // Ymax
                    || !file.geti(record.polyline->num_parts, endian::little)
                    || !file.geti(record.polyline->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "missing data for polyline record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polyline;
                        return false;
                    }
                    
                    if(!(record.polyline->parts = new (std::nothrow) int32_t[record.polyline->num_parts]))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polyline parts in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polyline; // dtor cleans up the rest
                        return false;
                    }
                    
                    if(!(record.polyline->points = new (std::nothrow) shp::point[record.polyline->num_points]))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polyline points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polyline; // dtor cleans up the rest
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polyline->num_parts; ++i)
                    {
                        int32_t& part = record.polyline->parts[i];
                        if(!file.geti(part, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "missing polyline parts in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            delete record.polyline; // dtor cleans up the rest
                            return false;
                        }
                    }
                    
                    for(int32_t i = 0; i < record.polyline->num_points; ++i)
                    {
                        shp::point& point = record.polyline->points[i];
                        if(!file.getf(point.x, endian::little)
                        || !file.getf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "missing polyline points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            delete record.polyline; // dtor cleans up the rest
                            return false;
                        }
                    }
                    
                    record.shape = record.polyline;
                }
                else if(record.type == shp::shape::polygon)
                {
                    if(!(record.polygon = new (std::nothrow) shp::polygon))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polygon record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    
                    if(!file.getf(record.polygon->box[0], endian::little) // Xmin
                    || !file.getf(record.polygon->box[1], endian::little) // Ymin
                    || !file.getf(record.polygon->box[2], endian::little) // Xmax
                    || !file.getf(record.polygon->box[3], endian::little) // Ymax
                    || !file.geti(record.polygon->num_parts, endian::little)
                    || !file.geti(record.polygon->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "missing data for polygon record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polygon;
                        return false;
                    }
                    
                    if(!(record.polygon->parts = new (std::nothrow) int32_t[record.polygon->num_parts]))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polygon parts in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polygon; // dtor cleans up the rest
                        return false;
                    }
                    
                    if(!(record.polygon->points = new (std::nothrow) shp::point[record.polygon->num_points]))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for polygon points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.polygon; // dtor cleans up the rest
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polygon->num_parts; ++i)
                    {
                        int32_t& part = record.polygon->parts[i];
                        if(!file.geti(part, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "missing polygon parts in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            delete record.polygon; // dtor cleans up the rest
                            return false;
                        }
                    }
                    
                    for(int32_t i = 0; i < record.polygon->num_points; ++i)
                    {
                        shp::point& point = record.polygon->points[i];
                        if(!file.getf(point.x, endian::little)
                        || !file.getf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "missing polygon points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            delete record.polygon; // dtor cleans up the rest
                            return false;
                        }
                    }
                    
                    record.shape = record.polygon;
                }
                else if(record.type == shp::shape::multipoint)
                {
                    if(!(record.multipoint = new (std::nothrow) shp::multipoint))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for multipoint record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    
                    if(!file.getf(record.multipoint->box[0], endian::little) // Xmin
                    || !file.getf(record.multipoint->box[1], endian::little) // Ymin
                    || !file.getf(record.multipoint->box[2], endian::little) // Xmax
                    || !file.getf(record.multipoint->box[3], endian::little) // Ymax
                    || !file.geti(record.multipoint->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "missing data for multipoint record in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.multipoint;
                        return false;
                    }
                    
                    if(!(record.multipoint->points = new (std::nothrow) shp::point[record.multipoint->num_points]))
                    {
                        if(errorlog) fprintf(stderr, "could not allocate memory for multipoint points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        delete record.multipoint; // dtor cleans up the rest
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.multipoint->num_points; ++i)
                    {
                        shp::point& point = record.multipoint->points[i];
                        if(!file.getf(point.x, endian::little)
                        || !file.getf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "missing multipoint points in shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            delete record.multipoint; // dtor cleans up the rest
                            return false;
                        }
                    }
                    
                    record.shape = record.multipoint;
                }
                else
                {
                    if(errorlog) fprintf(stderr, "unsupported record type for reading: %s (record #%i in %s)\n", shp::typestr(record.type), record.number, filename.c_str());
                    file.seek((record.length * 2) - 4, true);
                }
                
                records.push_back(record);
            } // !file.eof();
            
            file.close();
            return true;
        }
        
        if(errorlog) fprintf(stderr, "could not open file for reading: %s\n", filename.c_str());
        return false;
    }
    
    bool shp::save(const std::string& filename, const bool errorlog)
    {
        reshp::file file;
        
        if(file.open(filename, reshp::file::mode::write | reshp::file::mode::binary))
        {
            file.clear();
            
            // Write shapefile header
            if(!file.puti(header.identifier, endian::big)
            || !file.write(reinterpret_cast<char*>(header.unused), sizeof(header.unused))
            || !file.puti(header.length, endian::big)       // Rewritten after writing records to file
            || !file.puti(header.version, endian::little)
            || !file.puti(header.type, endian::little)
            || !file.putf(header.box[0], endian::little)
            || !file.putf(header.box[1], endian::little)
            || !file.putf(header.box[2], endian::little)
            || !file.putf(header.box[3], endian::little)
            || !file.putf(header.z_range[0], endian::little)
            || !file.putf(header.z_range[1], endian::little)
            || !file.putf(header.z_range[0], endian::little)
            || !file.putf(header.m_range[1], endian::little))
            {
                if(errorlog) fprintf(stderr, "could not write shapefile header to: %s\n", filename.c_str());
                return false;
            }
            
            int32_t filelen = 100; // Header size in bytes
            
            // Write shapefile records
            for(unsigned i = 0; i < records.size(); ++i)
            {
                reshp::shp::record& record = records[i];
                long header_start = file.tell();
                int32_t recordlen = 0;
                
                if(!record.number)
                    record.number = (i + 1);
                
                if(!file.puti(record.number, endian::big)
                || !file.puti(record.length, endian::big)
                || !file.puti(record.type, endian::little))
                {
                    if(errorlog) fprintf(stderr, "could not write shapefile record header to: %s (index %u)\n", filename.c_str(), i);
                    return false;
                }
                else recordlen += 2; // Type (Number and Length excluded)
                
                if(record.type == shp::shape::null)
                {
                    // Empty shape (no additional data)
                }
                else if(record.type == shp::shape::point)
                {
                    if(!record.point)
                    {
                        if(errorlog) fprintf(stderr, "missing record point data for record: #%i\n", record.number);
                        return false;
                    }
                    
                    if(!file.putf(record.point->x, endian::little)
                    || !file.putf(record.point->y, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "could not write point record to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    else recordlen += 8;
                }
                else if(record.type == shp::shape::polyline)
                {
                    if(!record.polyline)
                    {
                        if(errorlog) fprintf(stderr, "missing record polyline data for record: #%i\n", record.number);
                        return false;
                    }
                    
                    if(!file.putf(record.polyline->box[0], endian::little) // Xmin
                    || !file.putf(record.polyline->box[1], endian::little) // Ymin
                    || !file.putf(record.polyline->box[2], endian::little) // Xmax
                    || !file.putf(record.polyline->box[3], endian::little) // Ymax
                    || !file.puti(record.polyline->num_parts, endian::little)
                    || !file.puti(record.polyline->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "could not write polyline record to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    else recordlen += 20;
                    
                    if(!record.polyline->parts)
                    {
                        if(errorlog) fprintf(stderr, "missing parts in polyline record: #%i\n", record.number);
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polyline->num_parts; ++i)
                    {
                        int32_t& part = record.polyline->parts[i];
                        if(!file.puti(part, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "could not write polyline parts to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            return false;
                        }
                        else recordlen += 2;
                    }
                    
                    if(!record.polyline->points)
                    {
                        if(errorlog) fprintf(stderr, "missing points in polyline record: #%i\n", record.number);
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polyline->num_points; ++i)
                    {
                        shp::point& point = record.polyline->points[i];
                        if(!file.putf(point.x, endian::little)
                        || !file.putf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "could not write polyline points to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            return false;
                        }
                        else recordlen += 8;
                    }
                }
                else if(record.type == shp::shape::polygon)
                {
                    if(!record.polygon)
                    {
                        if(errorlog) fprintf(stderr, "missing record polygon data for record: #%i\n", record.number);
                        return false;
                    }
                    
                    if(!file.putf(record.polygon->box[0], endian::little) // Xmin
                    || !file.putf(record.polygon->box[1], endian::little) // Ymin
                    || !file.putf(record.polygon->box[2], endian::little) // Xmax
                    || !file.putf(record.polygon->box[3], endian::little) // Ymax
                    || !file.puti(record.polygon->num_parts, endian::little)
                    || !file.puti(record.polygon->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "could not write polygon record to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    else recordlen += 20;
                    
                    if(!record.polygon->parts)
                    {
                        if(errorlog) fprintf(stderr, "missing parts in polygon record: #%i\n", record.number);
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polygon->num_parts; ++i)
                    {
                        int32_t& part = record.polygon->parts[i];
                        if(!file.puti(part, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "could not write polygon parts to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            return false;
                        }
                        else recordlen += 2;
                    }
                    
                    if(!record.polygon->points)
                    {
                        if(errorlog) fprintf(stderr, "missing points in polygon record: #%i\n", record.number);
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.polygon->num_points; ++i)
                    {
                        shp::point& point = record.polygon->points[i];
                        if(!file.putf(point.x, endian::little)
                        || !file.putf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "could not write polygon points to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            return false;
                        }
                        else recordlen += 8;
                    }
                }
                else if(record.type == shp::shape::multipoint)
                {
                    if(!record.multipoint)
                    {
                        if(errorlog) fprintf(stderr, "missing record multipoint data for record: #%i\n", record.number);
                        return false;
                    }
                    
                    if(!file.putf(record.multipoint->box[0], endian::little) // Xmin
                    || !file.putf(record.multipoint->box[1], endian::little) // Ymin
                    || !file.putf(record.multipoint->box[2], endian::little) // Xmax
                    || !file.putf(record.multipoint->box[3], endian::little) // Ymax
                    || !file.puti(record.multipoint->num_points, endian::little))
                    {
                        if(errorlog) fprintf(stderr, "could not write multipoint record to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                        return false;
                    }
                    else recordlen += 20;
                    
                    if(!record.multipoint->points)
                    {
                        if(errorlog) fprintf(stderr, "missing points in multipoint record: #%i\n", record.number);
                        return false;
                    }
                    
                    for(int32_t i = 0; i < record.multipoint->num_points; ++i)
                    {
                        shp::point& point = record.multipoint->points[i];
                        if(!file.putf(point.x, endian::little)
                        || !file.putf(point.y, endian::little))
                        {
                            if(errorlog) fprintf(stderr, "could not write multipoint points to shapefile: %s (record #%i)\n", filename.c_str(), record.number);
                            return false;
                        }
                        else recordlen += 8;
                    }
                }
                else
                {
                    if(errorlog) fprintf(stderr, "unsupported record type for writing: %s (record #%i)\n", shp::typestr(record.type), record.number);
                    
                    char data[record.length * 2];
                    memset(data, 0, sizeof(data));
                    
                    if(!file.write(data, sizeof(data)))
                        return false;
                }
                
                if(!record.length)
                    record.length = recordlen;
                
                if(record.length != recordlen)
                {
                    if(errorlog) fprintf(stderr, "record header length mismatch for record: #%i (%i, expected %i)\n", record.number, record.length, recordlen);
                    return false;
                }
                else filelen += (record.length + 4) * 2; // Include Number and Length
                
                // Rewrite record length in record header
                long record_end = file.tell();
                file.seek(header_start + 4); // record.length
                file.puti(recordlen, endian::big);
                file.seek(record_end);
            } // records
            
            // Rewrite file length in file header
            
            file.seek(24); // header.length
            file.puti((header.length = (filelen / 2)), endian::big);
            
            file.close();
            return true;
        }
        
        if(errorlog) fprintf(stderr, "could not open file for writing: %s\n", filename.c_str());
        return false;
    }
}
