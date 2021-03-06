#ifndef HITABLELISTHPP
#define HITABLELISTHPP

#include "hitable.hpp"

class hitable_list: public hitable {
    public:
        hitable_list() {}
        hitable_list(hitable **l, int n) { list = l; list_size = n; }
        virtual bool hit(
            ray const & r,
            double t_min,
            double t_max,
            hit_record & hr
        ) const;
        hitable **list;
        int list_size;
};


bool hitable_list::hit(
            ray const & r,
            double t_min,
            double t_max,
            hit_record & hr
        ) const {

    hit_record temp_hr;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; ++i) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_hr)) {
            hit_anything = true;
            closest_so_far = temp_hr.t;
            hr = temp_hr;
        }
    }
    return hit_anything;
}

#endif