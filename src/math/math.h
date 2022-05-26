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

mat4_t mat4_new(float value);
float vec2_dot(vec2_t a, vec2_t b);

mat4_t mat4_ortho(float left, float right, float top, float bottom, float far, float near);
mat4_t mat4_ortho_aspect(float aspect, float size, float far, float near);

mat4_t mat4_scale(mat4_t matrix, vec3_t vector);
mat4_t mat4_translate(mat4_t matrix, vec3_t vector);

mat4_t mat4_multiply(mat4_t mat1, mat4_t mat2);
mat4_t mat4_inverse(mat4_t matrix);


float lerp(float a, float b, float f);