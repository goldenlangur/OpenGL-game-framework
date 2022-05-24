#pragma once

typedef struct
{
    uint32_t id;
    vec2_t size;
    int channel_count;
    unsigned char* data;
}texture_t;

typedef struct 
{
    vec2_t uv[4];
}sub_texture_t;

typedef struct
{
    vec3_t pos;
    vec4_t color;
    vec2_t uv;
    float tex_index;
}vertex_t;

typedef struct 
{
    vec4_t clear_color;

    mat4_t view_mat;
    mat4_t proj_mat;

    uint32_t max_quad_count;
    
    uint32_t vbo;
    uint32_t vao;
    uint32_t ibo;

    uint32_t shader;

    uint32_t* index_buffer;
    uint32_t index_count;

    vertex_t* vertex_buffer;
    vertex_t* vertex_buffer_ptr;

    texture_t tex_slots[16];
    uint32_t tex_slot_count;
}renderer_s;


void renderer_init(renderer_s* renderer);
void renderer_start(renderer_s* renderer);
void renderer_end(renderer_s* renderer);
void renderer_exit(renderer_s* renderer);

void renderer_draw_texture(
    renderer_s* renderer, 
    texture_t* texture, 
    sub_texture_t* sub_texture, 
    const vec3_t pos, 
    const vec3_t size,
    const vec4_t color);

void renderer_draw_quad(
    renderer_s* renderer, 
    const vec3_t pos, 
    const vec3_t size, 
    const vec4_t color);


texture_t texture_create_from_file(const char* path);
texture_t texture_create_from_data(unsigned char* data, vec2_t size);

sub_texture_t sub_texture_create(vec2_t tex_size, vec2_t coords, vec2_t tilesize);

const char* glsl_load_from_file(const char* path);

