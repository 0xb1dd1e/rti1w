#ifndef CAMERAHPP
#define CAMERAHPP

#include "ray.hpp"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}

class camera {
    public:
        camera( vec3 const & lookFrom,
                vec3 const & lookAt,
                vec3 const & vUp,
                double vfov, 
                double aspect,
                double aperature,
                double focusDist) {    
            lens_radius = aperature / 2.0;
            double theta = vfov * M_PI/180.0;   
            double half_height = tan(theta/2.0);
            double half_width = aspect * half_height;      
            origin = lookFrom;
            w = unit_vector(lookFrom - lookAt);
            u = unit_vector(cross(vUp,w));
            v = cross(w,u);
            lower_left_corner = origin - half_width*focusDist*u - half_height*focusDist*v - focusDist*w;
            horizontal = 2.0 * half_width * focusDist*u;
            vertical = 2.0 * half_height * focusDist*v;
        }

        ray get_ray(double s, double t) {
            vec3 rd = lens_radius*random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(
                origin + offset, 
                lower_left_corner 
                + (s * horizontal) 
                + (t * vertical) 
                - origin
                - offset
            );            
        }

        vec3 origin;
        vec3 lower_left_corner ;
        vec3 horizontal;
        vec3 vertical;
        vec3 u,v,w;
        double lens_radius;
};

#endif