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

#include "shp.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

bool argcmp(char* arg, const char* opt1, const char* opt2 = NULL)
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

int main(int argc, char* argv[])
{
    char build_version[] = "0.3.2";
    char build_date[] = "YYYY-MM-DD";
    
    memcpy(build_date + 0, __DATE__ + 7, 4);
    memcpy(build_date + 8, __DATE__ + 4, 2);

    if(build_date[5] == ' ')
        build_date[5] = '0';
    
         if(!memcmp(__DATE__, "Jan", 3)) memcpy(build_date + 5, "01", 2);
    else if(!memcmp(__DATE__, "Feb", 3)) memcpy(build_date + 5, "02", 2);
    else if(!memcmp(__DATE__, "Mar", 3)) memcpy(build_date + 5, "03", 2);
    else if(!memcmp(__DATE__, "Apr", 3)) memcpy(build_date + 5, "04", 2);
    else if(!memcmp(__DATE__, "May", 3)) memcpy(build_date + 5, "05", 2);
    else if(!memcmp(__DATE__, "Jun", 3)) memcpy(build_date + 5, "06", 2);
    else if(!memcmp(__DATE__, "Jul", 3)) memcpy(build_date + 5, "07", 2);
    else if(!memcmp(__DATE__, "Aug", 3)) memcpy(build_date + 5, "08", 2);
    else if(!memcmp(__DATE__, "Sep", 3)) memcpy(build_date + 5, "09", 2);
    else if(!memcmp(__DATE__, "Oct", 3)) memcpy(build_date + 5, "10", 2);
    else if(!memcmp(__DATE__, "Nov", 3)) memcpy(build_date + 5, "11", 2);
    else if(!memcmp(__DATE__, "Dec", 3)) memcpy(build_date + 5, "12", 2);
    
    char filename[strlen(argv[0]) + 1];
    char* pathsep = strrchr(argv[0], '/');
    strcpy(filename, pathsep ? pathsep + 1 : argv[0]);
    reshp::shp shapefile;
    
    for(int i = 1; i < argc; ++i)
    {
        if(argcmp(argv[i], "--help", "-h"))
        {
            if(argc > i + 1)
            {
                if(argcmp(argv[i + 1], "list", "l"))
                {
                    printf("%s <shapefile> --list\n", filename);
                    printf("List various information about shapefile.\n");
                }
                else if(argcmp(argv[i + 1], "add", "a"))
                {
                    printf("%s <shapefile> --add <type> [property]...\n", filename);
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
                else if(argcmp(argv[i + 1], "output", "o"))
                {
                    printf("%s <shapefile> --output [filename]\n", filename);
                    printf("Output changes made to the shapefile to specified filename.\n");
                }
                else if(argcmp(argv[i + 1], "save"))
                {
                    printf("%s <shapefile> --save [output]\n", filename);
                    printf("Save any changes made to the shapefile (thus overwriting the original contents).\n");
                    printf("Overwriting the original shapefile can be prevented by specifying an output filename.\n");
                }
                else if(argcmp(argv[i + 1], "subtract", "s"))
                {
                    printf("%s <shapefile> --subtract <input>\n", filename);
                    printf("Subtract input shapefile shapes from shapes in the shapefile.\n");
                }
                else
                {
                    printf("Unrecognized parameter: %s\n", argv[i + 1]);
                    printf("Try '%s --help' for more information\n", filename);
                }
            }
            else
            {
                printf("Usage: %s [option] [shapefile <parameter>...]\n", filename);
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
            
            exit(EXIT_SUCCESS);    
        }
        else if(argcmp(argv[i], "--version"))
        {
            printf("reshp (reshape) version %s (built on %s)\n", build_version, build_date);
            printf("Command-line tool for manipulating ESRI Shapefiles.\n");
            exit(EXIT_SUCCESS);
        }
    }
    
    printf("%s: missing parameter\n", filename);
    printf("Try '%s --help' for more information\n", filename);
    
    return EXIT_SUCCESS;
}
