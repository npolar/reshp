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

#include "handler.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace reshp
{
    bool handler::argcmp(const char* arg, const char* opt1, const char* opt2)
    {
        if(arg && opt1)
        {
            if(strcmp(arg, opt1) == 0)
                return true;
            
            if(opt2 && (strcmp(arg, opt2) == 0))
                return true;
        }
        
        return false;
    }
    
    void handler::handle_help(const char* topic)
    {
        if(!topic)
        {
            printf("Usage: %s [option] [shapefile <parameter>...]\n", exe_name_.c_str());
            printf("Command-line tool for manipulating Esri Shapefiles.\n");
            
            printf("\nAvailable options:\n");
            printf("  -h, --help [parameter]            display help text (about specified parameter) and exit\n");
            printf("      --version                     output version information and exit\n");
            
            printf("\nShapefile parameters:\n");
            printf("  -a, --add <type> <property>...    add record with specified type and properties to shapefile\n");
            printf("  -l, --list                        list various information about the shapefile\n");
            printf("  -o, --output <filename>           output (save) modified shapefile to specified filename\n");
            printf("      --save [filename]             save any modifications made to the shapefile\n");
            printf("  -s, --subtract <input>            subtract input shapefile shapes from shapefile\n");
        }
        else if(argcmp(topic, "add", "a"))
        {
            printf("%s <shapefile> --add <type> [property]...\n", exe_name_.c_str());
            printf("Add record with specified type and properties to shapefile.\n");
            
            printf("\nRecord types:\n");
            printf("  null                              empty placeholder shape\n");
            printf("  point <x> <y>                     single 2D point\n");
            printf("  polyline <mbr> <parts> <points>   polygonal path of 2D points\n");
            printf("  polygon <mbr> <parts> <points>    filled polygon of 2D points\n");
            printf("  multipoint <mbr> <points>         collection of 2D points\n");
            
            printf("\nProperties:\n");
            printf("  <measure> measure floating number\n");
            printf("  <mbr>     bounding box with space separated floating numbers: <xmin> <ymin> <xmax> <ymax>\n");
            printf("  <parts>   number of parts followed by space separated point indices representing each part\n");
            printf("  <points>  number of points followed by space separated point coordinates: <x> <y>\n");
            printf("  <x>       x coordinate as floating number\n");
            printf("  <y>       y coordinate as floating number\n");
            printf("  <z>       z coordinate as floating number\n");
        }
        else if(argcmp(topic, "list", "l"))
        {
            printf("%s <shapefile> --list\n", exe_name_.c_str());
            printf("List various information about shapefile.\n");
        }
        else if(argcmp(topic, "output", "o"))
        {
            printf("%s <shapefile> --output [filename]\n", exe_name_.c_str());
            printf("Output changes made to the shapefile to specified filename.\n");
        }
        else if(argcmp(topic, "save"))
        {
            printf("%s <shapefile> --save [output]\n", exe_name_.c_str());
            printf("Save any changes made to the shapefile (thus overwriting the original contents).\n");
            printf("Overwriting the original shapefile can be prevented by specifying an output filename.\n");
        }
        else if(argcmp(topic, "subtract", "s"))
        {
            printf("%s <shapefile> --subtract <input>\n", exe_name_.c_str());
            printf("Subtract input shapefile shapes from shapes in the shapefile.\n");
        }
        else
        {
            printf("Unrecognized parameter: %s\n", topic);
            printf("Try '%s --help' for more information\n", exe_name_.c_str());
        }
    }
    
    void handler::handle_list(const std::string& shapefile)
    {
        reshp::shp shp;
        if(!shp.load(shapefile))
            return;
        
        printf("Filename:   %s\n", shapefile.c_str());
        printf("Version:    %i\n", shp.header.version);
        printf("Type:       %s\n", shp::typestr(shp.header.type));
        printf("Bounding:   [%.4f, %.4f] [%.4f, %.4f]\n", shp.header.box[0], shp.header.box[1], shp.header.box[2], shp.header.box[3]);
        printf("Records:    %lu\n\n", shp.records.size());
        
        for(unsigned i = 0; i < shp.records.size(); ++i)
        {
            const char* type = shp::typestr(shp.records[i].type);
            
            printf("  #%05i (%s)", shp.records[i].number, type);
            
            int32_t points = 0, rings = 0;
            
            if(shp.records[i].polygon)
            {
                rings = shp.records[i].polygon->num_parts;
                points = shp.records[i].polygon->num_points;
            }
            
            if(points)  printf(":%*s%5i point%c", int(13 - strlen(type)), " ", points, (points == 1 ? ' ' : 's'));
            if(rings)   printf(", %5i ring%c", rings, (rings == 1 ? ' ' : 's'));
            
            printf("\n");
        }
    }
    
    handler::handler(int argc, char** argv, const char* version) :
        build_date_("0000-00-00"),
        exe_name_(argv[0]),
        version_(version)
    {
        size_t separator = exe_name_.rfind('/');
        std::string builddate(__DATE__);
        
        if(separator != std::string::npos)
            exe_name_.erase(0, separator + 1);
        
        build_date_.replace(0, 4, builddate.substr(7, 4));
        build_date_.replace(8, 2, builddate.substr(4, 2));
        
        if(build_date_[5] == ' ')
            build_date_[5] = '0';
        
             if(builddate.substr(0, 3) == "Jan") build_date_.replace(5, 2, "01");
        else if(builddate.substr(0, 3) == "Feb") build_date_.replace(5, 2, "02");
        else if(builddate.substr(0, 3) == "Mar") build_date_.replace(5, 2, "03");
        else if(builddate.substr(0, 3) == "Apr") build_date_.replace(5, 2, "04");
        else if(builddate.substr(0, 3) == "May") build_date_.replace(5, 2, "05");
        else if(builddate.substr(0, 3) == "Jun") build_date_.replace(5, 2, "06");
        else if(builddate.substr(0, 3) == "Jul") build_date_.replace(5, 2, "07");
        else if(builddate.substr(0, 3) == "Aug") build_date_.replace(5, 2, "08");
        else if(builddate.substr(0, 3) == "Sep") build_date_.replace(5, 2, "09");
        else if(builddate.substr(0, 3) == "Oct") build_date_.replace(5, 2, "10");
        else if(builddate.substr(0, 3) == "Nov") build_date_.replace(5, 2, "11");
        else if(builddate.substr(0, 3) == "Dec") build_date_.replace(5, 2, "12");
        
        for(int i = 1; i < argc; ++i)
        {
            if(argcmp(argv[i], "--help", "-h"))
            {
                handle_help(argc > i + 1 ? argv[i + 1] : NULL);
                exit(EXIT_SUCCESS);
            }
            else if(argcmp(argv[i], "--version"))
            {
                printf("reshp (reshape) version %s (built on %s)\n", version_.c_str(), build_date_.c_str());
                printf("Command-line tool for manipulating ESRI Shapefiles.\n");
                exit(EXIT_SUCCESS);
            }
            else if(argc > i + 1)
            {
                std::string shapefile(argv[i]);
                
                if(argcmp(argv[i + 1], "--list", "l"))
                {
                    handle_list(shapefile);
                }
                else
                {
                    printf("unrecognized parameter: %s\n", argv[i + 1]);
                    printf("Try '%s --help' for more information\n", exe_name_.c_str());
                }
                
                exit(EXIT_SUCCESS);
            }
        }
        
        printf("%s: missing parameter\n", exe_name_.c_str());
        printf("Try '%s --help' for more information\n", exe_name_.c_str());
        exit(EXIT_SUCCESS);
    }
    
    int handler::run()
    {
        return EXIT_SUCCESS;
    }
}