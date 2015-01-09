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

#ifndef RESHP_HANDLER_HPP_
#define RESHP_HANDLER_HPP_

#include "shp.hpp"
#include "shx.hpp"
#include <string>

namespace reshp
{
    class handler
    {
    private:
        std::string     build_date_;    // Compilation date (yyyy-mm-dd)
        std::string     exe_name_;      // Program executable name
        bool            verbose_;
        std::string     version_;
        
        bool argcmp(const char* arg, const char* opt1, const char* opt2 = NULL);
        
        void help(const char* topic = NULL);
        void list(const std::string& shapefile, const bool full = false);
        void subtract(const std::string& basefile, const std::string& maskfile, const char* outputfile = NULL);
        void validate(const std::string& shapefile);
        
    public:
        handler(int argc, char** argv, const char* version);
        int run();
    };
}

#endif // RESHP_HANDLER_HPP_
