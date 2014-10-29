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

#ifndef RESHP_SHAPE_HPP_
#define RESHP_SHAPE_HPP_

namespace reshp
{
    struct aabb
    {
        struct
        {
            double x, y, z;
        } min, max;
            
        aabb();
        bool inside(const reshp::aabb&) const;
        bool intersects(const reshp::aabb&) const;
    };
    
    class shape
    {
    protected:
        shape(const char* type);
        
    public:
        virtual ~shape();
        
        reshp::aabb aabb;
        const char* type;
            
        virtual bool inside(const reshp::shape*) const;
        virtual bool intersects(const reshp::shape*) const;
        virtual void update();
    };
}

#endif // RESHP_SHAPE_HPP_
