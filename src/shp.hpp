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

#ifndef RESHP_SHP_HPP_
#define RESHP_SHP_HPP_

#include "file.hpp"

#include <stdint.h>
#include <vector>
#include <string>

namespace reshp
{
    typedef file::endian endian;
    
    class shp
    {
    public:
        static const int32_t    IDENTIFIER  = 9994;
        static const int32_t    VERSION     = 1000;
        static const double     NO_DATA     = -10e+40;
        static const double     DATA_LIMIT  = -10e+38; 
        
        static const char*      typestr(const int32_t type);
        
        struct shape
        {
            enum type
            {
                null            = 0,    // implemented read, write
                point           = 1,    // implemented read, write
                polyline        = 3,    // implemented read, write
                polygon         = 5,    // implemented read, write
                multipoint      = 8,    // implemented read, write
                zpoint          = 11,
                zpolyline       = 13,
                zpolygon        = 15,
                zmultipoint     = 18,
                mpoint          = 21,
                mpolyline       = 23,
                mpolygon        = 25,
                mmultipoint     = 28,
                multipatch      = 31
            };
        };
        
        struct point
        {
            double x, y;            // little
            
            point(const double x = 0.0, const double y = 0.0);
        };
        
        struct polyline
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]

            polyline();
        };
        
        struct polygon
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]

            polygon();
        };
        
        struct multipoint
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_points; // little
            shp::point* points;     // [num_points]

            multipoint();
        };
        
        struct zpoint
        {
            double x, y, z, m;      // little

            zpoint(const double x = 0.0, const double y = 0.0, const double z = 0.0, const double m = 0.0);
        };
        
        struct zpolyline
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]
            double      z_range[2]; // little (Zmin, Zmax)
            double*     z_array;    // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            zpolyline();
        };
        
        struct zpolygon
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]
            double      z_range[2]; // little (Zmin, Zmax)
            double*     z_array;    // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            zpolygon();
        };
        
        struct zmultipoint
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_points; // little
            shp::point* points;     // [num_points]
            double      z_range[2]; // little (Zmin, Zmax)
            double*     z_array;    // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            zmultipoint();
        };
        
        struct mpoint
        {
            double x, y, m;         // little

            mpoint(const double x, const double y, const double m);
        };
        
        struct mpolyline
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            mpolyline();
        };
        
        struct mpolygon
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            shp::point* points;     // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            mpolygon();
        };
        
        struct mmultipoint
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_points; // little
            shp::point* points;     // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            mmultipoint();
        };
        
        struct multipatch
        {
            double      box[4];     // little (Xmin, Ymin, Xmax, Ymax)
            int32_t     num_parts;  // little
            int32_t     num_points; // little
            int32_t*    parts;      // [num_parts]
            int32_t*    part_types; // [num_parts]
            shp::point* points;     // [num_points]
            double      z_range[2]; // little (Zmin, Zmax)
            double*     z_array;    // [num_points]
            double      m_range[2]; // little (Mmin, Mmax)
            double*     m_array;    // [num_points]

            multipatch();
        };
        
        struct record
        {
            int32_t     number;     // big (1-based)
            int32_t     length;     // big (16-bit words)
            int32_t     type;       // little
            
            shp::shape*       shape;          
            shp::point*       point;
            shp::polyline*    polyline;
            shp::polygon*     polygon;
            shp::multipoint*  multipoint;
            shp::zpoint*      zpoint;
            shp::zpolyline*   zpolyline;
            shp::zpolygon*    zpolygon;
            shp::zmultipoint* zmultipoint;
            shp::mpoint*      mpoint;
            shp::mpolyline*   mpolyline;
            shp::mpolygon*    mpolygon;
            shp::mmultipoint* mmultipoint;
            shp::multipatch*  multipatch;

            record();
        };
        
        struct
        {
            union
            {
                char data[100];
                
                struct
                {
                    int32_t identifier;         // big (0x0000270A)
                    int32_t unused[5];          // big
                    int32_t length;             // big (16-bit words)
                    int32_t version;            // little (1000)
                    int32_t type;               // little
                    
                    struct
                    {
                        struct
                        {
                            double x, y;        // little
                        } min, max;
                    } mbr;
                    
                    struct
                    {
                        double min, max;        // little
                    } z, m;
                };
            };
        } header;
        
        ~shp();
        
        void    free();
        bool    load(const std::string& filename);
        bool    save(const std::string& filename);
        
        std::vector<shp::record> records;
    };
}

#endif // RESHP_SHP_HPP_
