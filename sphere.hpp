#ifndef SPHEREHPP
#define SPHEREHPP

#include "hitable.hpp"

class sphere: public hitable {
    public:
        sphere() : pMat(nullptr) {}
        sphere(vec3 const & cen, double r, material const * const mat) : center(cen), radius(r), pMat(mat) {};
        virtual bool hit(
            ray const & r,
            double t_min,
            double t_max,
            hit_record & hr
        ) const;
        vec3 center;
        double radius;
        material const * pMat;
};

bool sphere::hit(
            ray const & r,
            double t_min,
            double t_max,
            hit_record & hr
        ) const {


    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b*b - a*c;
    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double temp = (-b - root) / a;
        if (temp < t_max && temp > t_min) {
            hr.t = temp;
            hr.p = r.point_at_parameter(temp);
            hr.normal = (hr.p - center) / radius;
            hr.pMat = pMat;
            return true;
        }
        temp = (-b + root) / a;
        if (temp < t_max && temp > t_min) {
            hr.t = temp;
            hr.p = r.point_at_parameter(temp);
            hr.normal = (hr.p - center) / radius;
            hr.pMat = pMat;
            return true;
        }
    }
    return false;    
}

#endif
