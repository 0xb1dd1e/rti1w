#ifndef RAYHPP
#define RAYHPP
#include "vec3.hpp"

class ray {
    public:
        ray() {}
        ray( vec3 const & a, vec3 const & b) { A = a; B = b; }
        vec3 const & origin() const {return A; }
        vec3 const & direction() const { return B; }
        vec3 point_at_parameter(double const t) const {
             return A + t * B;
        }

        vec3 A;
        vec3 B;
};

#endif
