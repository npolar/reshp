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
    
    void handler::help(const char* topic)
    {
        if(!topic)
        {
            printf("Usage: %s [option]... [shapefile <parameter>...]\n", exe_name_.c_str());
            printf("Command-line tool for manipulating ESRI Shapefiles.\n");
            
            printf("\nAvailable options:\n");
            printf("  -h, --help [topic]                display help text (about specified topic) and exit\n");
            printf("      --version                     output version information and exit\n");
            printf("  -v, --verbose                     verbosely output performed manipulation operations\n");
            
            printf("\nShapefile parameters:\n");
            //printf("  -a, --add <type> <property>...    add record with specified type and properties to shapefile\n");
            printf("  -l, --list                        list various information about the shapefile\n");
            printf("  -L, --list-full                   list full information about the shapefile, including coordinates\n");
            printf("  -o, --output <filename>           output (save) modified shapefile to specified filename\n");
            printf("  -s, --subtract <input>            subtract input shapefile shapes from shapefile\n");
            printf("  -V, --validate                    validate the shapefile with a grade ranging from A to F\n");
        }
        /*
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
        */
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
        else if(argcmp(topic, "subtract", "s"))
        {
            printf("%s <shapefile> --subtract <input>\n", exe_name_.c_str());
            printf("Subtract input shapefile shapes from shapes in the shapefile.\n");
        }
        else if(argcmp(topic, "verbose", "v"))
        {
            printf("%s --verbose [shapefile <parameter>...]\n", exe_name_.c_str());
            printf("Enable verbose output of certain manipulation operations.\n");
        }
        else if(argcmp(topic, "validate", "V"))
        {
            printf("%s --validate\n", exe_name_.c_str());
            printf("Validate the shapefile, and output one of the following grades:\n");
            printf("  A: Valid ESRI Shapefile, which completely complies with the specifications\n");
            printf("  B: Valid ESRI Shapefile, with valid data and clean shapes according to the specifications\n");
            printf("  C: Valid ESRI Shapefile, with valid data and no self-intersecting shapes\n");
            printf("  D: Valid ESRI Shapefile, with no broken records or missing data\n");
            printf("  E: Valid ESRI Shapefile, with broken records or missing data\n");
            printf("  F: Not a valid ESRI Shapefile\n");
            printf("\n");
            printf("  If an output filename is specified, the validation will be performed on this file\n");
        }
        else
        {
            printf("Unrecognized help topic: %s\n", topic);
            printf("Try '%s --help' for more information\n", exe_name_.c_str());
        }
    }
    
    handler::handler(int argc, char** argv, const char* version) :
        build_date_("0000-00-00"),
        exe_name_(argv[0]),
        verbose_(false),
        version_(version)
    {
        size_t separator = exe_name_.rfind('/');
        std::string builddate(__DATE__);
        
        if(separator != std::string::npos)
            exe_name_.erase(0, separator + 1);
        
        build_date_.replace(0, 4, builddate.substr(7, 4));
        build_date_.replace(8, 2, builddate.substr(4, 2));
        
        if(build_date_[8] == ' ')
            build_date_[8] = '0';
        
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
        
        // Parse options
        if(argc >= 2)
        {
            for(int o = 1; o < argc; ++o)
            {
                if(argcmp(argv[o], "--help", "-h"))
                {
                    help(argc > o + 1 ? argv[o + 1] : NULL);
                    break;
                }
                else if(argcmp(argv[o], "--version"))
                {
                    printf("reshp (reshape) version %s (built on %s)\n", version_.c_str(), build_date_.c_str());
                    printf("Command-line tool for manipulating ESRI Shapefiles.\n");
                    
                    printf("\nWritten by the Norwegian Polar Institute (http://npolar.no/)\n");
                    printf("This is free software, and comes with absolutely NO WARRANTY\n");
                    break;
                }
                else if(argcmp(argv[o], "--verbose", "-v"))
                {
                    verbose_ = true;
                }
                else if(argc > o + 1)
                {
                    enum
                    {
                        action_none         = 0x00,
                        action_cleanup      = 0x01,
                        action_output       = 0x02,
                        action_subtract     = 0x04,
                        action_validate     = 0x08
                    };
                    
                    std::string filename(argv[o]);
                    std::string input, output;
                    unsigned action = action_none;
                    
                    // Parse parameters
                    for(int p = o + 1; p < argc; ++p)
                    {
                        if(argcmp(argv[p], "--list", "-l"))
                        {
                            list(filename, false);
                        }
                        else if(argcmp(argv[p], "--list-full", "-L"))
                        {
                            list(filename, true);
                        }
                        else if(argcmp(argv[p], "--subtract", "-s"))
                        {
                            if(argc > p + 1)
                            {
                                input = argv[p + 1];
                                action |= action_subtract;
                                ++p;
                            }
                            else
                            {
                                printf("missing input shapefile for subtract masking\n");
                                printf("Try '%s --help subtract' for more information\n", exe_name_.c_str());
                            }
                        }
                        else if(argcmp(argv[p], "--output", "-o"))
                        {
                            if(argc > p + 1)
                            {
                                output = std::string(argv[p + 1]);
                                action |= action_output;
                                ++p;
                            }
                            else
                            {
                                printf("missing filename for shapefile output\n");
                                printf("Try '%s --help output' for more information\n", exe_name_.c_str());
                            }
                        }    
                        else if(argcmp(argv[p], "--validate", "-V"))
                        {
                            action |= action_validate;
                        }
                        else
                        {
                            printf("unrecognized parameter: %s\n", argv[p]);
                            printf("Try '%s --help' for more information\n", exe_name_.c_str());
                        }
                    }
                    
                    // Perform subtraction if specified
                    if(action & action_subtract)
                        subtract(filename, input, (action & action_output ? output.c_str() : NULL));
                    
                    // Perform validation if specified
                    if(action & action_validate)
                        validate(action & action_output ? output : filename);
                    
                    break;
                }
            }
        }
        else
        {
            printf("%s: missing parameter\n", exe_name_.c_str());
            printf("Try '%s --help' for more information\n", exe_name_.c_str());
        }
    }
    
    int handler::run()
    {
        return EXIT_SUCCESS;
    }
}
