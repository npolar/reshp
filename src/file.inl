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

namespace reshp
{
    template<typename T>
    T file::endian::ensure(const file::endian::type endianness, const T& value)
    {
        const unsigned size = sizeof(T);
        union { T w; char b[size]; } src, dst;
        src.w = dst.w = value;
        
        if(endianness != file::endian::native && file::endian::native != file::endian::unknown)
        {
            for(unsigned i = 0; i < size; ++i)
                dst.b[i] = src.b[size - 1 - i];
        }
        
        return dst.w;
    }

    template<typename T>
    bool file::geti(T& number, const file::endian::type endianness)
    {
        if(file_ && (modes_ & file::mode::read))
        {
            if(modes_ & file::mode::binary)
            {
                const unsigned size = sizeof(T);
                union { T w; char b[size]; };
                
                if(read(b, size))
                {
                    number = file::endian::ensure(endianness, w);
                    return true;
                }
            }
            else
            {
                long int tmp = 0;
                
                if(fscanf(file_, "%li", &tmp) == 1)
                {
                    number = static_cast<T>(tmp);
                    return true;
                }
            }
        }
        
        return false;
    }

    template<typename T>
    bool file::getf(T& number, const file::endian::type endianness)
    {
        if(file_ && (modes_ & file::mode::read))
        {
            if(modes_ & file::mode::binary)
            {
                const unsigned size = sizeof(T);
                union { T w; char b[size]; };
                
                if(read(b, size))
                {
                    number = file::endian::ensure(endianness, w);
                    return true;
                }
            }
            else
            {
                double tmp = 0.0;
                
                if(fscanf(file_, "%lf", &tmp) == 1)
                {
                    number = static_cast<T>(tmp);
                    return true;
                }
            }
        }
        
        return false;
    }

    template<typename T>
    bool file::puti(const T& number, const file::endian::type endianness)
    {
        if(file_ && (modes_ & file::mode::write))
        {
            if(modes_ & file::mode::binary)
            {
                const unsigned size = sizeof(T);
                union { T w; char b[size]; };
                w = file::endian::ensure(endianness, number);
                
                if(write(b, size))
                    return true;
            }
            else
            {
                long int tmp = static_cast<long int>(number);
                int chars = fprintf(file_, "%li ", tmp);
                
                if(chars > 0)
                {
                    size_ += chars;
                    return true;
                }
            }
        }
            
        return false;
    }

    template<typename T>
    bool file::putf(const T& number, const file::endian::type endianness)
    {
        if(file_ && (modes_ & file::mode::write))
        {
            if(modes_ & file::mode::binary)
            {
                const unsigned size = sizeof(T);
                union { T w; char b[size]; };
                w = file::endian::ensure(endianness, number);
                
                if(write(b, size))
                    return true;
            }
            else
            {
                double tmp = static_cast<double>(number);
                int chars = fprintf(file_, "%lf ", tmp);
                
                if(chars > 0)
                {
                    size_ += chars;
                    return true;
                }
            }
        }
            
        return false;
    }
}
