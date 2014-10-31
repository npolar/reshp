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
#include <string>

namespace reshp
{
    class handler
    {
    private:
        std::string     build_date_;    // Compilation date (yyyy-mm-dd)
        std::string     exe_name_;      // Program executable name
        std::string     version_;
        
        bool argcmp(const char* arg, const char* opt1, const char* opt2 = NULL);
        
        void handle_help(const char* topic = NULL);
        void handle_list(const std::string& shapefile);
        
    public:
        handler(int argc, char** argv, const char* version);
        int run();
    };
}

#endif // RESHP_HANDLER_HPP_
