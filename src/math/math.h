#pragma once

typedef struct
{
    float x;
    float y;
}vec2_t;

typedef struct
{
    float x;
    float y;
    float z;
}vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}vec4_t;

typedef struct 
{
    float data[4][4];
} mat4_t;

mat4_t mat4_new(const float value);
float vec2_dot(const vec2_t a, const vec2_t b);

mat4_t mat4_ortho(const float left, const float right, const float top, const float bottom, const float far, const float near);
mat4_t mat4_ortho_aspect(const float aspect, const float size, const float far, float near);

mat4_t mat4_scale(const mat4_t matrix, const vec3_t vector);
mat4_t mat4_translate(const mat4_t matrix, const vec3_t vector);

mat4_t mat4_multiply(const mat4_t mat1, const mat4_t mat2);
mat4_t mat4_inverse(const mat4_t matrix);


float lerp(const float a, const float b, const float f);