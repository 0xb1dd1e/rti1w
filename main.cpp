#include <iostream>
#include "vec3.hpp"
#include "sphere.hpp"
#include "hitablelist.hpp"
#include "camera.hpp"
#include "material.hpp"
#include <float.h>

vec3 farplane(ray const & r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

vec3 colourizeNormal(hit_record const & hr) {
    return 0.5f * vec3(hr.normal.x()+1, hr.normal.y()+1, hr.normal.z()+1);    
}

vec3 colour(ray const &r, hitable const *world, int depth) {
    hit_record hr;
    if (world->hit(r, 0.001f, MAXFLOAT, hr)) {
        ray scattered;
        vec3 attentuation;
        if (depth < 50 && hr.pMat->scatter(r, hr, attentuation, scattered)) {
            return attentuation * colour(scattered, world, depth+1);
        } else {
            return vec3(0,0,0);
        }
    } else {
        return farplane(r);
    }
}

hitable *randomScene() {
    int n = 500;
    int maxTiny = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(.5f,.5f,.5f)));
    int i = 1;
    for (int a = -11; a < 11 && i <= maxTiny; ++a) {
        for (int b = -11; b < 11 && i <= maxTiny; ++b) {
            float chooseMat = drand48();
            vec3 center(a+0.9f*drand48(), 0.2, b+0.9f*drand48());
            if ((center - vec3(4,0.2,0)).length() > 0.9) {
                if (chooseMat < 0.8) {
                    list[i++] = new sphere(
                        center, 0.2f, 
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
                        center, 0.2,
                        new metal(
                            vec3(
                                0.5f * (1 + drand48()),
                                0.5f * (1 + drand48()),
                                0.5f * (1 + drand48())
                            ),
                            0.5f * drand48()                
                        )
                    );
                } else {
                    list[i++] = new sphere(center, 0.2f, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3( 0, 1, 0), 1.0f, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new lambertian(vec3(0.4f,0.2f,0.1f)));
    list[i++] = new sphere(vec3( 4, 1, 0), 1.0f, new metal(vec3(0.7f,0.6f,0.5f),0.0));

    return new hitable_list(list,i);
}

int main () {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  
    /*
    hitable *list[5];

    //float R = cos(M_PI/4.0f);
    //list[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0,0,1)));
    //list[1] = new sphere(vec3( R, 0, -1), R, new lambertian(vec3(1,0,0)));

    
    list[0] = new sphere(vec3(0, 0, -1), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new sphere(vec3(1,0,-1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.5f));
    list[3] = new sphere(vec3(-1,0,-1), 0.5f, new dielectric(1.5f));
    list[4] = new sphere(vec3(-1,0,-1), -0.45f, new dielectric(1.5f));
    */

    //hitable * world = new hitable_list(list,5);

    hitable * world = randomScene(); 

    vec3 lookFrom(12,2,4);
    vec3 lookAt(0,0.5,0);
    float dist_to_focus = (lookFrom-lookAt).length();
    float aperature = 0.0;
    camera cam(
        lookFrom,
        lookAt,
        vec3( 0, 1, 0),
        20, 
        float(nx)/float(ny),
        aperature,
        dist_to_focus
    );

    for (int j = ny-1; j >= 0 ; --j) {
        for (int i = 0; i < nx; ++i) {

            vec3 col(0,0,0);

            for (int s = 0; s < ns; ++s) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(2.0);
                col += colour(r, world, 0);
            }

            col /= float(ns);

            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
         }
    }
}