#ifndef HITABLEHPP
#define HITABLEHPP

#include "ray.hpp"

class material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material const * pMat;
};

class hitable {
    public:
        virtual bool hit(
            ray const & r,
            float t_min,
            float t_max,
            hit_record & rec
        ) const = 0;
};

#endif
