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

#include "handler.hpp"

int main(int argc, char* argv[])
{
    // TODO: Update version number
    reshp::handler reshp(argc, argv, "0.4.1");
    return reshp.run();
}
