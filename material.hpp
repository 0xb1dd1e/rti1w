#ifndef MATERIALHPP
#define MATERIALHPP

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable.hpp"

class material {
    public:
        virtual bool scatter(ray const & r_in, hit_record const & hr , vec3 & attentuation, ray & scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(vec3 const & a) : albedo(a) {}

        virtual bool scatter(ray const & r_in, hit_record const & hr , vec3 & attentuation, ray & scattered) const {
            vec3 target = hr.p + hr.normal + random_in_unit_sphere();
            scattered = ray(hr.p, target - hr.p);
            attentuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material {
    public:
        metal(vec3 const & a, double f) : albedo(a) { 
            fuzz = (fabs(f) < 1.0) ? f : 1.0;
        }

        virtual bool scatter(ray const & r_in, hit_record const & hr , vec3 & attentuation, ray & scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), hr.normal);
            scattered = ray(hr.p, reflected + fuzz * random_in_unit_sphere());
            attentuation = albedo;
            return (dot(scattered.direction(), hr.normal) > 0);
        }

        vec3 albedo;
        double fuzz;
};

double schlick(double cosine, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1-r0) * pow((1.0 - cosine), 5.0);
}

class dielectric : public material {
    public: 
        dielectric(double ri) : ref_idx(ri) {}

        virtual bool scatter(ray const & r_in, hit_record const & hr , vec3 & attentuation, ray & scattered) const {
            vec3 outward_normal;
            vec3 reflected = reflect(unit_vector(r_in.direction()), hr.normal);
            double ni_over_nt;
            attentuation = vec3(1,1,1);
            vec3 refracted = vec3(0,0,0);
            double reflect_prob;
            double cosine;
            if (dot(r_in.direction(), hr.normal) > 0) {
                outward_normal = -hr.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx * dot(r_in.direction(), hr.normal) / r_in.direction().length();
            } else {
                outward_normal = hr.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dot(r_in.direction(), hr.normal) / r_in.direction().length();
            }
            if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
                reflect_prob = schlick(cosine, ref_idx);
            } else {
                reflect_prob = 1.0;
            }
            if (drand48() >= reflect_prob) {
                scattered = ray(hr.p, refracted);
            } else {
                scattered = ray(hr.p, reflected);
            }
            return true;
        }

        double ref_idx;
};

#endif