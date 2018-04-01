#include <iostream>
#include "sphere.hpp"
#include "hitablelist.hpp"
#include <float.h>

vec3 farplane(ray const & r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

vec3 colourizeNormal(hit_record const & hr) {
    return 0.5f * vec3(hr.normal.x()+1, hr.normal.y()+1, hr.normal.z()+1);    
}

vec3 colour(ray const &r, hitable const *world) {
    hit_record hr;
    if (world->hit(r, 0.0f, MAXFLOAT, hr)) {
        return colourizeNormal(hr);
    } else {
        return farplane(r);
    }
}

int main () {
    int nx = 200;
    int ny = 100;
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    
    vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    vec3 horizontal(4.0f, 0.0f, 0.0f);
    vec3 vertical(0.0f, 2.0f, 0.0f);
    vec3 origin(0.0f, 0.0f, 0.0f);

    hitable *list[2];

    list[0] = new sphere(vec3(0, 0, -1), 0.5f);
    list[1] = new sphere(vec3(0, -10.5f, -1), 10);

    hitable * world = new hitable_list(list,2);

    for (int j = ny-1; j >= 0 ; j--) {
        for (int i = 0; i < nx; i++) {

            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            ray r(
                origin, 
                lower_left_corner + (u * horizontal) + (v * vertical)
            );

            vec3 p = r.point_at_parameter(2.0);

            vec3 col = colour(r, world);

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
         }
    }
}