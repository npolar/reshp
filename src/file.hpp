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

#ifndef RESHP_FILE_HPP_
#define RESHP_FILE_HPP_

#include <ctime>
#include <cstdio>
#include <string>
#include <cstring>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace reshp
{
    class file
    {
    public:
        struct mode
        {
            enum
            {
                read    = 0x01,
                write   = 0x02,
                binary  = 0x04
            };
            
            typedef uint8_t flags;
        };
            
        struct eol
        {
            enum type
            {
                detect,
                cr,             // \r
                lf,             // \n
                crlf,           // \r\n
                lfcr            // \n\r
            };
        };
        
        struct endian
        {
            enum type
            {
                unknown,
                little,
                big
            };
            
            static type detect();
            static const type native;
            
            template<typename T>
            static T ensure(const file::endian::type endianness, const T& value);
        };
        
        struct time
        {
            struct month
            {
                enum
                {
                    january = 1,
                    february,
                    march,
                    april,
                    may,
                    june,
                    july,
                    august,
                    september,
                    october,
                    november,
                    december
                };
            };
            
            union
            {
                char data[5];
                
                struct
                {
                    uint32_t
                    year: 14,   // 0 - 16383
                    month: 4,   // 0 - 15
                    day: 5,     // 0 - 31
                    hour: 5,    // 0 - 31
                    minute: 6,  // 0 - 63
                    second: 6;  // 0 - 63
                };
            } accessed, modified;
        };

    private:
        FILE*               file_;
        file::endian::type  endian_;
        file::eol::type     eol_;
        file::mode::flags   modes_;
        size_t              size_;
        
    public:
        file();
        ~file();
        
        static bool         clear(const std::string& filename);
        static bool         exists(const std::string& filename);
        static bool         remove(const std::string& filename);
        static bool         rename(const std::string& oldname, const std::string& newname);
        static size_t       size(const std::string& filename);
        static file::time   timestamp(const std::string& filename);
        
        bool                clear();
        void                close();
        bool                eof() const;
        bool                open(const std::string& filename, const file::mode::flags = file::mode::read, const file::eol::type = file::eol::detect);
        bool                opened() const;
        bool                seek(const long position, const bool relative = false);
        size_t              size() const;
        long                tell() const;
        
        bool                read(char* dst, size_t bytes);                                      // Read a given amount of bytes, automatically terminated by EOF
        bool                gets(std::string&);                                                 // Read a null-terminated (binary mode) or whitespace-separated (text mode) string
        bool                getline(std::string&);                                              // Read a single line of text, automatically removing leading whitespace (text mode only)
        
        template<typename T>
        bool geti(T& number, const file::endian::type endianness = file::endian::native);       // Read a single integer of appropriate type size and endianness (binary mode) or separated by whitespace (text mode)
        
        template<typename T>
        bool getf(T& number, const file::endian::type endianness = file::endian::native);       // Read a single floating number of appropriate type size and endianness (binary mode) or separated by whitespace (text mode)
        
        file&               operator>> (char&);
        file&               operator>> (int8_t&);
        file&               operator>> (int16_t&);
        file&               operator>> (int32_t&);
        file&               operator>> (uint8_t&);
        file&               operator>> (uint16_t&);
        file&               operator>> (uint32_t&);
        file&               operator>> (float&);
        file&               operator>> (double&);
        file&               operator>> (std::string&);
        
        #if __WORDSIZE == 64
        file&               operator>> (int64_t&);
        file&               operator>> (uint64_t&);
        #endif
        
        bool                write(const char* src, size_t bytes);                               // Write a give amount of bytes to the file
        bool                puts(const std::string&);                                           // Write a null-terminated (binary mode) or white-space separated (text mode) string
        bool                putline(const std::string&);                                        // Write the appropriate EOL-marker followed by the specified string (text mode only)
        
        template<typename T>
        bool puti(const T& number, const file::endian::type endianness = file::endian::native); // Write a single integer of appropriate type size and endianess (binary mode) or separated by whitespace (text mode)
        
        template<typename T>
        bool putf(const T& number, const file::endian::type endianness = file::endian::native); // Write a single fractional number of appropriate type size and endianess (binary mode) or separated by a whitespace (text mode)
        
        file&               operator<< (const char);
        file&               operator<< (const int8_t);
        file&               operator<< (const int16_t);
        file&               operator<< (const int32_t);
        file&               operator<< (const uint8_t);
        file&               operator<< (const uint16_t);
        file&               operator<< (const uint32_t);
        file&               operator<< (const float);
        file&               operator<< (const double);
        file&               operator<< (const std::string&);
        
        #if __WORDSIZE == 64
        file&               operator<< (const int64_t);
        file&               operator<< (const uint64_t);
        #endif
    };
}

#include "file.inl"
#endif // RESHP_FILE_HPP_
