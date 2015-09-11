#ifndef vec_H
#define vec_H
#include "math.h"
class vec3 {
  public:
    float x, y, z;
    vec3();
    vec3(float x, float y, float z);
    float operator*(vec3 const &v);
    vec3 cross(vec3 const &v);
    vec3 operator+(vec3 const &v);
    vec3 operator-(vec3 const &v);
    vec3 operator*(float const s);
    vec3& operator=(vec3 const &v);
    float length();
    vec3 norm();
};

class vec2 {
  public:
    float x, y;
    vec2();
    vec2(float x, float y);
    float operator*(vec2 const &v);
    vec2 cross(vec2 const &v);
    vec2 operator+(vec2 const &v);
    vec2 operator-(vec2 const &v);
    vec2 operator*(float const s);
    vec2& operator=(vec2 const &v);
    float length();
    vec2 norm();
};

#endif