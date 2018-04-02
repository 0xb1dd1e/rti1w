#ifndef VEC3HPP
#define VEC3HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3{
    public:
        vec3() {}

        vec3(float e0, float e1, float e2) {
            e[0] = e0; e[1] = e1; e[2] = e2;
        }

        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }

        inline vec3 const & operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float & operator[](int i) { return e[i]; }

        inline vec3 & operator+=(vec3 const & v2);
        inline vec3 & operator-=(vec3 const & v2);
        inline vec3 & operator*=(vec3 const & v2);
        inline vec3 & operator/=(vec3 const & v2);
        inline vec3 & operator*=(float const t);
        inline vec3 & operator/=(float const t);
        
        inline float length() const {
            return sqrt( e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        }

        inline float squared_length() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        
        inline void make_unit_vector();

        float e[3];
};

inline std::istream& operator>>(std::istream & is, vec3 & v) {
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream & os, vec3 & v) {
    os << v.e[0] << v.e[1] << v.e[2];
    return os;
}

inline void vec3::make_unit_vector() {
    float k = 1.0f / sqrt( e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline vec3 operator+(vec3 const & v1, vec3 const & v2) {
    return vec3(
        v1.e[0] + v2.e[0],
        v1.e[1] + v2.e[1],
        v1.e[2] + v2.e[2]
    );
}

inline vec3 operator-(vec3 const & v1, vec3 const & v2) {
    return vec3(
        v1.e[0] - v2.e[0],
        v1.e[1] - v2.e[1],
        v1.e[2] - v2.e[2]
    );
}

inline vec3 operator*(vec3 const & v1, vec3 const & v2) {
    return vec3(
        v1.e[0] * v2.e[0],
        v1.e[1] * v2.e[1],
        v1.e[2] * v2.e[2]
    );
}

inline vec3 operator/(vec3 const & v1, vec3 const & v2) {
    return vec3(
        v1.e[0] / v2.e[0],
        v1.e[1] / v2.e[1],
        v1.e[2] / v2.e[2]
    );
}

inline vec3 operator*(float const t, vec3 const & v) {
    return vec3(
        t * v.e[0],
        t * v.e[1],
        t * v.e[2]
    );
}

inline vec3 operator*(vec3 const & v, float const t) {
    return vec3(
        t * v.e[0],
        t * v.e[1],
        t * v.e[2]
    );
}

inline vec3 operator/(vec3 const & v, float const t) {
    return vec3(
        v.e[0] / t,
        v.e[1] / t,
        v.e[2] / t
    );
}

inline float dot(vec3 const & v1, vec3 const & v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(vec3 const & v1, vec3 const & v2) {
    return vec3(
        v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]    
    );
}

inline vec3 & vec3::operator+=(vec3 const & v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return  *this;
}

inline vec3 & vec3::operator-=(vec3 const & v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return  *this;
}

inline vec3 & vec3::operator*=(vec3 const & v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return  *this;
}

inline vec3 & vec3::operator/=(vec3 const & v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return  *this;
}

inline vec3 & vec3::operator*=(float const t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return  *this;
}

inline vec3 & vec3::operator/=(float const t) {
    float const k = 1.0f / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return  *this;
}

inline vec3 unit_vector(vec3 const & v) {
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

inline vec3 reflect(vec3 const & v, vec3 const & n) {
    return v - 2.0f * dot(v,n)*n;
}

inline bool refract(vec3 const & v, vec3 const & n, float ni_over_nt, vec3 & refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n*dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

#endif