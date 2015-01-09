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

#include "shx.hpp"

#include <cstdio>
#include <cstring>

namespace reshp
{
    shx::record::record() :
        offset(-1),
        length(-1)
    {
    }
    
    shx::shx()
    {
        header.identifier = shx::IDENTIFIER;
        memset(header.unused, 0, sizeof(header.unused));
        header.length = 50;
        header.version = shx::VERSION;
        header.type = shp::shape::null;
        header.box[0] = header.box[1] = header.box[2] = header.box[3] = 0.0;
        header.z_range[0] = header.z_range[1] = 0.0;
        header.m_range[0] = header.m_range[1] = 0.0;
    }
    
    shx::~shx()
    {
        free();
    }
    
    void shx::free()
    {
        records.clear();
        memset(&header, 0, sizeof(header));
    }
    
    bool shx::load(const reshp::shp& shp, const bool errorlog)
    {
        if(shp.header.version != shx::VERSION)
        {
            if(errorlog) fprintf(stderr, "unsupported shapefile version for shx loading: %i (expected: %i)\n", shp.header.version, shx::VERSION);
            return false;
        }
        
        header.identifier = shp.header.identifier;
        memcpy(header.unused, shp.header.unused, sizeof(header.unused));
        header.length = 50; // Header length (16-bit words)
        header.version = shp.header.version;
        header.type = shp.header.type;
        memcpy(header.box, shp.header.box, sizeof(header.box));
        memcpy(header.z_range, shp.header.z_range, sizeof(header.z_range));
        memcpy(header.m_range, shp.header.m_range, sizeof(header.m_range));
        
        for(unsigned r = 0; r < shp.records.size(); ++r)
        {
            shx::record record;
            record.offset = header.length;
            record.length = shp.records[r].length;
            header.length += 4; // offset, length
            records.push_back(record);
        }
        
        return true;
    }
    
    bool shx::save(const std::string& filename, const bool errorlog)
    {
        reshp::file file;
        
        if(file.open(filename, reshp::file::mode::write | reshp::file::mode::binary))
        {
            file.clear();
            
            // Write shapefile index header
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
                if(errorlog) fprintf(stderr, "could not write shapefile index header to: %s\n", filename.c_str());
                return false;
            }
            
            // Write shapefile index records
            for(unsigned i = 0; i < records.size(); ++i)
            {
                reshp::shx::record& record = records[i];
                
                if(!file.puti(record.offset, endian::big)
                || !file.puti(record.length, endian::big))
                {
                    if(errorlog) fprintf(stderr, "could not write shapefile index record to: %s (index %u)\n", filename.c_str(), i);
                    return false;
                }
            } // records
            
            file.close();
            return true;
        }
        
        if(errorlog) fprintf(stderr, "could not open file for writing: %s\n", filename.c_str());
        return false;
    }
}
