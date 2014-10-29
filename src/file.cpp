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

#include "file.hpp"

namespace reshp
{
    const file::endian::type file::endian::native = file::endian::detect();

    file::endian::type file::endian::detect()
    {
        union { uint32_t dw; uint8_t b[4]; }; dw = 0xAABBCCDD;
        
        if(b[0] == 0xAA && b[1] == 0xBB && b[2] == 0xCC && b[3] == 0xDD)
            return file::endian::big;
            
        if(b[0] == 0xDD && b[1] == 0xCC && b[2] == 0xBB && b[3] == 0xAA)
            return file::endian::little;
        
        return file::endian::unknown;
    }

    file::file() :
        file_(NULL),
        endian_(file::endian::native),
        eol_(file::eol::detect),
        modes_(file::mode::read),
        size_(0)
    {
    }

    file::~file()
    {
        close();
    }

    bool file::clear(const std::string& filename)
    {
        if(file::exists(filename))
        {
            FILE* fp = fopen(filename.c_str(), "wb");
            
            if(fp)
            {
                fclose(fp);
                return true;
            }
        }
        
        return false;
    }

    bool file::exists(const std::string& filename)
    {
        FILE* fp = fopen(filename.c_str(), "rb");
        
        if(fp)
        {
            fclose(fp);
            return true;
        }
        
        return false;
    }

    bool file::remove(const std::string& filename)
    {
        return (::remove(filename.c_str()) == 0 ? true : false);
    }

    bool file::rename(const std::string& oldname, const std::string& newname)
    {
        return (::rename(oldname.c_str(), newname.c_str()) == 0 ? true : false);
    }

    size_t file::size(const std::string& filename)
    {
        FILE* fp = fopen(filename.c_str(), "rb");
        
        if(fp)
        {
            fseek(fp, 0, SEEK_END);
            long int bytes = ftell(fp);
            
            if(bytes >= 0)
            {
                fclose(fp);
                return static_cast<size_t>(bytes);
            }
            
            fclose(fp);
        }
        
        return 0;
    }

    file::time file::timestamp(const std::string& filename)
    {
        file::time ft;
        
        memset(ft.accessed.data, 0, sizeof(ft.accessed.data));
        memset(ft.modified.data, 0, sizeof(ft.modified.data));
        
        if(file::exists(filename))
        {
            bool success = false;
            
        #if defined(__linux__)
            struct stat attr;
            success = (::stat(filename.c_str(), &attr) == 0);
        #elif defined(__WIN32)
            struct _stat attr;
            success = (::_stat(filename, &attr) == 0);
        #endif
            
            if(success)
            {
                struct tm* ltaccessed   = localtime(&attr.st_atime);
                ft.accessed.year        = ltaccessed->tm_year + 1900;
                ft.accessed.month       = ltaccessed->tm_mon + 1;
                ft.accessed.day         = ltaccessed->tm_mday;
                ft.accessed.hour        = ltaccessed->tm_hour;
                ft.accessed.minute      = ltaccessed->tm_min;
                ft.accessed.second      = ltaccessed->tm_sec;
                
                struct tm* ltmodified   = localtime(&attr.st_mtime);
                ft.modified.year        = ltmodified->tm_year + 1900;
                ft.modified.month       = ltmodified->tm_mon + 1;
                ft.modified.day         = ltmodified->tm_mday;
                ft.modified.hour        = ltaccessed->tm_hour;
                ft.modified.minute      = ltaccessed->tm_min;
                ft.modified.second      = ltaccessed->tm_sec;
            }
        }
        
        return ft;
    }

    bool file::clear()
    {
        if(file_ && (modes_ & file::mode::write))
        {
            char fmodes[4] = { 'w', 0, 0, 0 };
            uint8_t m = 1;
            
            if(modes_ & file::mode::read)
                fmodes[m++] = 'r';
                
            if(modes_ & file::mode::binary)
                fmodes[m++] = 'b';
                
            if((file_ = freopen(NULL, fmodes, file_)))
            {
                size_ = 0;
                return true;
            }
        }
        
        return false;
    }

    void file::close()
    {
        if(file_)
        {
            fclose(file_);
            file_ = NULL;
        }
        
        endian_ = file::endian::native;
        eol_    = file::eol::detect;
        modes_  = file::mode::read;
        size_   = 0;
    }

    bool file::eof() const
    {
        if(file_)
        {
            if(feof(file_))
                return true;
            
            char c = fgetc(file_);
            if(c == EOF) return true;
            else ungetc(c, file_);
        }
        
        return false;
    }

    bool file::open(const std::string& filename, const file::mode::flags modes, const file::eol::type eol)
    {
        close();
        
        char fmodes[4] = { 0, 0, 0, 0 };
        uint8_t m = 0;
        
        if(modes & file::mode::read)
            fmodes[m++] = 'r';
        
        if(modes & file::mode::write)
            fmodes[m++] = 'a';
        
        if(modes & file::mode::binary)
            fmodes[m++] = 'b';
        
        if((file_ = fopen(filename.c_str(), fmodes)))
        {
            if(!(modes & file::mode::binary))
            {
                if(eol == file::eol::detect)
                {
                    fseek(file_, 0, SEEK_SET);
                    
                    for(char c = fgetc(file_); c != EOF; c = fgetc(file_))
                    {
                        if(c == '\r')
                        {
                            eol_ = (fgetc(file_) == '\n' ? file::eol::crlf : file::eol::cr);
                            ungetc(c, file_);
                            break;
                        }
                        else if(c == '\n')
                        {
                            eol_ = (fgetc(file_) == '\r' ? file::eol::lfcr : file::eol::lf);
                            ungetc(c, file_);
                            break;
                        }
                    }
                    
                    if(eol_ == file::eol::detect)
                    {
                    #if defined(__WIN32)
                        eol_ = file::eol::crlf;
                    #else
                        eol_ = file::eol::lf;
                    #endif
                    }
                    
                    clearerr(file_);
                }
                else eol_ = eol;
            }
            
            fseek(file_, 0, SEEK_END);
            size_ = static_cast<size_t>(ftell(file_));
            
            if(!((modes_ = modes) & file::mode::write))
                fseek(file_, 0, SEEK_SET);
            
            return true;
        }
        
        return false;
    }

    bool file::opened() const
    {
        return (file_ ? true : false);
    }

    bool file::seek(const long position, const bool relative)
    {
        if(file_)
        {
            return (fseek(file_, position, (relative ? SEEK_CUR : SEEK_SET)) == 0);
        }
        
        return false;
    }

    size_t file::size() const
    {
        return size_;
    }

    long file::tell() const
    {
        if(file_)
            return ftell(file_);
        
        return -1;
    }

    bool file::read(char* dst, size_t bytes)
    {
        if(file_ && (modes_ & file::mode::read))
            if(fread(dst, bytes, 1, file_))
                return true;
            
        return false;
    }

    bool file::gets(std::string& str)
    {
        if(file_ && (modes_ & file::mode::read))
        {
            size_t len = 0;
            char* buf = NULL;
            
            if(modes_ & file::mode::binary)
            {
                for(char c = fgetc(file_); c != EOF; c = fgetc(file_), ++len)
                    if(c == '\0')
                        break;
                        
                if(len++ && (fseek(file_, -len, SEEK_CUR) == 0))
                {
                    if((buf = new char[len]()))
                    {
                        if(!fread(buf, len, 1, file_))
                        {
                            delete [] buf;
                            return false;
                        }
                        
                        str = buf;
                        delete [] buf;
                    }
                }
                
                return true;
            }
            else
            {
                if((buf = new char[size_]()))
                {
                    if(fscanf(file_, "%s", buf) == 1)
                    {
                        str = buf;
                        delete [] buf;
                        return true;
                    }
                        
                    delete [] buf;
                }
            }
        }
        
        return false;
    }

    bool file::getline(std::string& str)
    {
        if(file_ && (modes_ & file::mode::read) && !(modes_ & file::mode::binary))
        {
            char eol[2] = { 0, 0 };
            
            switch(eol_)
            {
            case file::eol::cr:     memcpy(eol, "\r", 1);   break;
            case file::eol::crlf:   memcpy(eol, "\r\r", 2); break;
            case file::eol::lf:     memcpy(eol, "\n", 1);   break;
            case file::eol::lfcr:   memcpy(eol, "\n\r", 2); break;
            default:
                return false;
            }
            
            // Remove leading whitespace characters
            for(char c = fgetc(file_); c != EOF; ++c)
            {
                if(c != ' ' && c != '\t' && c != '\v' && c != '\r' && c != '\n')
                {
                    ungetc(c, file_);
                    break;
                }
                else continue;
            }
            
            for(char c = fgetc(file_), cc; c != EOF; c = fgetc(file_))
            {
                if(c == eol[0])
                {
                    if(!eol[1] || (eol[1] && (cc = fgetc(file_)) == eol[1]))
                        break;
                    
                    else if(eol[1])
                        ungetc(cc, file_);
                }
                
                str += c;
            }
            
            return true;
        }
        
        return false;
    }

    file& file::operator>> (char& c)
    {
        read(&c, 1);
        return *this;
    }

    file& file::operator>> (int8_t& s)
    {
        geti(s);
        return *this;
    }

    file& file::operator>> (int16_t& s)
    {
        geti(s);
        return *this;
    }

    file& file::operator>> (int32_t& s)
    {
        geti(s);
        return *this;
    }

    file& file::operator>> (uint8_t& u)
    {
        geti(u);
        return *this;
    }

    file& file::operator>> (uint16_t& u)
    {
        geti(u);
        return *this;
    }

    file& file::operator>> (uint32_t& u)
    {
        geti(u);
        return *this;
    }

    file& file::operator>> (float& f)
    {
        getf(f);
        return *this;
    }

    file& file::operator>> (double& d)
    {
        getf(d);
        return *this;
    }

    file& file::operator>> (std::string& str)
    {
        gets(str);
        return *this;
    }

    #if __WORDSIZE == 64
    file& file::operator>> (int64_t& s)
    {
        geti(s);
        return *this;
    }

    file& file::operator>> (uint64_t& u)
    {
        geti(u);
        return *this;
    }
    #endif

    bool file::write(const char* src, size_t bytes)
    {
        if(file_ && (modes_ & file::mode::write))
        {
            if(fwrite(src, bytes, 1, file_))
            {
                size_ += bytes;
                return true;
            }
        }
        
        return false;
    }

    bool file::putline(const std::string& str)
    {
        if(file_ && (modes_ & file::mode::write) && !(modes_ & file::mode::binary))
        {
            if(ftell(file_) > 0)
            {
                switch(eol_)
                {
                case file::eol::cr:     size_ += fwrite("\r", 1, 1, file_);     break;
                case file::eol::crlf:   size_ += fwrite("\r\n", 1, 2, file_);   break;
                case file::eol::lf:     size_ += fwrite("\n", 1, 1, file_);     break;
                case file::eol::lfcr:   size_ += fwrite("\n\r", 1, 2, file_);   break;
                default:;
                }
            }
            
            if(fwrite(str.c_str(), str.length(), 1, file_))
            {
                size_ += str.length();
                return true;
            }
        }
        
        return false;
    }

    bool file::puts(const std::string& str)
    {
        if(file_ && (modes_ & file::mode::write))
        {
            if(fputs(str.c_str(), file_) != EOF)
            {
                size_ += str.length();
                
                if(fputc((modes_ & file::mode::binary) ? '\0' : ' ', file_) != EOF)
                {
                    size_ += 1;
                    return true;
                }
            }
        }
        
        return false;
    }

    file& file::operator<< (const char c)
    {
        write(&c, 1);
        return *this;
    }

    file& file::operator<< (const int8_t s)
    {
        puti(s);
        return *this;
    }

    file& file::operator<< (const int16_t s)
    {
        puti(s);
        return *this;
    }

    file& file::operator<< (const int32_t s)
    {
        puti(s);
        return *this;
    }

    file& file::operator<< (const uint8_t u)
    {
        puti(u);
        return *this;
    }

    file& file::operator<< (const uint16_t u)
    {
        puti(u);
        return *this;
    }

    file& file::operator<< (const uint32_t u)
    {
        puti(u);
        return *this;
    }

    file& file::operator<< (const float f)
    {
        putf(f);
        return *this;
    }

    file& file::operator<< (const double d)
    {
        putf(d);
        return *this;
    }

    file& file::operator<< (const std::string& str)
    {
        puts(str);
        return *this;
    }

    #if __WORDSIZE == 64
    file& file::operator<< (const int64_t s)
    {
        puti(s);
        return *this;
    }

    file& file::operator<< (const uint64_t u)
    {
        puti(u);
        return *this;
    }
    #endif
}
