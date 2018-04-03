#include <iostream>
#include "vec3.hpp"
#include "sphere.hpp"
#include "hitablelist.hpp"
#include "camera.hpp"
#include "material.hpp"
#include <float.h>
#include <stdlib.h>

vec3 farplane(ray const & r) {
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

vec3 colourizeNormal(hit_record const & hr) {
    return 0.5 * vec3(hr.normal.x()+1, hr.normal.y()+1, hr.normal.z()+1);    
}

vec3 colour(ray const &r, hitable const *world, int depth) {
    hit_record hr;
    if (world->hit(r, 0.001, DBL_MAX, hr)) {
        ray scattered;
        vec3 attentuation;
        if (depth < 50 && hr.pMat->scatter(r, hr, attentuation, scattered)) {
            return attentuation * colour(scattered, world, depth+1);
        }
    } else {
        return farplane(r);
    }
    return vec3(0,0,0);
}

hitable *randomScene() {
    int n = 500;
    int maxTiny = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(.5,.5,.5)));
    int i = 1;
    for (int a = -11; a < 11 && i <= maxTiny; ++a) {
        for (int b = -11; b < 11 && i <= maxTiny; ++b) {
            double chooseMat = drand48();
            vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center - vec3(4,0.2,0)).length() > 0.9) {
                if (chooseMat < 0.8) {
                    list[i++] = new sphere(
                        center, 0.2, 
                        new lambertian(
                            vec3(
                                drand48()*drand48(),
                                drand48()*drand48(),
                                drand48()*drand48()
                            )
                        )
                    );
                } else if (chooseMat < 0.95) {
                    list[i++] = new sphere(
                        center, 0.1,
                        new metal(
                            vec3(
                                0.5 * (1 + drand48()),
                                0.5 * (1 + drand48()),
                                0.5 * (1 + drand48())
                            ),
                            0.5 * drand48()                
                        )
                    );
                } else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3( 0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4,0.2,0.1)));
    list[i++] = new sphere(vec3( 4, 1, 0), 1.0, new metal(vec3(0.7,0.6,0.5),0.0));

    return new hitable_list(list,i);
}

int main () {

    srand48(0xb1dd1e);
    
    int nx = 600;
    int ny = 300;
    int ns = 500;
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  
    /*
    hitable *list[5];

    //double R = cos(M_PI/4.0);
    //list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0,0,1)));
    //list[1] = new sphere(vec3( R, 0, -1), R, new lambertian(vec3(1,0,0)));

    
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.5));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    */

    //hitable * world = new hitable_list(list,5);

    hitable * world = randomScene(); 

    vec3 lookFrom(12,2,4);
    vec3 lookAt(0,0.5,0);
    double dist_to_focus = (lookFrom-lookAt).length();
    double aperature = 0.2;
    camera cam(
        lookFrom,
        lookAt,
        vec3( 0, 1, 0),
        20, 
        double(nx)/double(ny),
        aperature,
        dist_to_focus
    );

    for (int j = ny-1; j >= 0 ; --j) {
        for (int i = 0; i < nx; ++i) {

            vec3 col(0,0,0);

            for (int s = 0; s < ns; ++s) {
                double u = double(i + drand48()) / double(nx);
                double v = double(j + drand48()) / double(ny);
                ray r = cam.get_ray(u,v);
                col += colour(r, world, 0);
            }

            col /= double(ns);

            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
         }
    }
}