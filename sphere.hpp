#ifndef SPHEREHPP
#define SPHEREHPP

#include "hitable.hpp"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 const & cen, float r) : center(cen), radius(r) {};
        virtual bool hit(
            ray const & r,
            float t_min,
            float t_max,
            hit_record & hr
        ) const;
        vec3 center;
        float radius;
};

bool sphere::hit(
            ray const & r,
            float t_min,
            float t_max,
            hit_record & hr
        ) const {


    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float root = sqrt(discriminant);
        float temp = (-b - root) / a;
        if (temp < t_max && temp > t_min) {
            hr.t = temp;
            hr.p = r.point_at_parameter(temp);
            hr.normal = (hr.p - center) / radius;
            return true;
        }
        temp = (-b + root) / a;
        if (temp < t_max && temp > t_min) {
            hr.t = temp;
            hr.p = r.point_at_parameter(temp);
            hr.normal = (hr.p - center) / radius;
            return true;
        }
    }
    return false;    
}

#endif
