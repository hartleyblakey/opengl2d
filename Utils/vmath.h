#ifndef MATH_H
#define MATH_H


typedef struct vec2{
    float x;
    float y;
} vec2;

typedef struct vec3{
    float x;
    float y;
    float z;
} vec3;

typedef float mat2[2][2];



vec2 add_vec2 (vec2 a, vec2 b);
vec3 add_vec3 (vec3 a, vec3 b);

vec2 mul_vec2 (vec2 a, vec2 b);
vec3 mul_vec3 (vec3 a, vec3 b);

mat2 mul_mat2 (mat2 a, mat2 b);

#define add(a, b) _Generic(a, vec2: add_vec2, vec3: add_vec3)(a, b)
#define mul(a, b) _Generic(a, vec2: mul_vec2, vec3: mul_vec3)(a, b)

#endif
