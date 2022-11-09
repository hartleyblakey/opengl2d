#include "vmath.h"

vec2 add_vec2 (vec2 a, vec2 b)
{
    vec2 r = {a.x+b.x, a.y+b.y};
    return r;
}

vec3 add_vec3 (vec3 a, vec3 b)
{
    vec3 r = {a.x+b.x, a.y+b.y, a.z + b.z};
    return r;
}

vec2 mul_vec2 (vec2 a, vec2 b)
{
    vec2 r = {a.x*b.x, a.y*b.y};
    return r;
}

vec3 mul_vec3 (vec3 a, vec3 b)
{
    vec3 r = {a.x*b.x, a.y*b.y, a.z*b.z};
    return r;
}


vec2 mul_mat2 (mat2 a, mat2 b)
{
    mat2 r;
    r[0][0] = a[0] * b[0] + a[1] * b[2];
    return r;
}
