#ifndef HITABLEHPP
#define HITABLEHPP

#include "ray.hpp"

class material;

struct hit_record {
    double t;
    vec3 p;
    vec3 normal;
    material const * pMat;
};

class hitable {
    public:
        virtual bool hit(
            ray const & r,
            double t_min,
            double t_max,
            hit_record & rec
        ) const = 0;
};

#endif
