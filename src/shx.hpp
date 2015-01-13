/* * * * * * * * * * * * *\
|*             ╔═╗ v0.4  *|
|* ╔═╦═╦═══╦═══╣ ╚═╦═╦═╗ *|
|* ║ ╔═╣ '╔╬═╗╚╣ ║ ║  '║ *|
|* ╚═╝ ╚═══╩═══╩═╩═╣ ╔═╝ *|
|* * * * * * * * * ╚═╝ * *|
|* Manipulation tool for *|
|*    ESRI Shapefiles    *|
|* * * * * * * * * * * * *|
|* http://www.npolar.no/ *|
\* * * * * * * * * * * * */

#ifndef RESHP_SHX_HPP_
#define RESHP_SHX_HPP_

#include "file.hpp"
#include "shp.hpp"

#include <stdint.h>
#include <vector>
#include <string>

namespace reshp
{
    typedef file::endian endian;
    
    class shx
    {
    public:
        static const int32_t    IDENTIFIER  = 9994;
        static const int32_t    VERSION     = 1000;
        
        struct record
        {
            int32_t     offset;     // big (16-bit words)
            int32_t     length;     // big (16-bit words)
            
            record();
        };
        
        struct
        {
            int32_t identifier;     // big (9994)
            int32_t unused[5];      // big
            int32_t length;         // big (16-bit words)
            int32_t version;        // little (1000)
            int32_t type;           // little
            double  box[4];         // little (Xmin, Ymin, Xmax, Ymax)
            double  z_range[2];     // little (Zmin, Zmax)
            double  m_range[2];     // little (Mmin, Mmax)
        } header;
        
        std::vector<shx::record> records;
        
        shx();
        ~shx();
        
        void free();
        bool load(const reshp::shp&, const bool errorlog = true);
        bool save(const std::string& filename, const bool errorlog = true);
    };
}

#endif // RESHP_SHX_HPP_
