#pragma once

typedef struct
{
    uint32_t id;
    vec2_t size;
    int channel_count;
    unsigned char *data;
} texture_t;

texture_t texture_create_from_file(const char *path);
texture_t texture_create_from_data(unsigned char *data, vec2_t size);
void texture_bind(texture_t *texture, uint32_t index);
void texture_delete(texture_t *texture);
void texture_update_data(texture_t *texture, unsigned char *data, vec2_t size);

typedef struct
{
    vec2_t uv[4];
    vec2_t tex_size;
    vec2_t sub_tex_size;
} sub_texture_t;

sub_texture_t sub_texture_create(vec2_t tex_size, vec2_t coords, vec2_t sub_tex_size);

typedef struct
{
    uint32_t id;
    const char *v_src;
    const char *f_src;
} shader_t;

shader_t shader_create(const char *vert_file_path, const char *frag_file_path);
void shader_bind(shader_t *shader);
void shader_delete(shader_t *shader);
void shader_set_uniform_mat4(shader_t *shader, const char *name, mat4_t matrix);
void shader_set_uniform_int(shader_t *shader, const char *name, int data);
void shader_set_uniform_int_arr(shader_t *shader, const char *name, int *data, uint32_t count);
const char *glsl_load_from_file(const char *path);

typedef struct
{
    uint32_t vbo;
    uint32_t vao;
    uint32_t ibo;
} vertex_array_t;

vertex_array_t vertex_array_create(void *vertices, size_t vertices_size, void *indices, size_t indices_size, bool dynamic);
void vertex_array_push_attribute(uint32_t index, uint32_t size, size_t stride, size_t offset);
void vertex_array_bind(vertex_array_t *vertex_array);
void vertex_array_delete(vertex_array_t *vertex_array);

void vertex_array_push_vertex_data(size_t size, void *data);
void vertex_array_push_index_data(size_t size, void *data);

typedef struct
{
    uint32_t fbo;
    uint32_t rbo;
    texture_t texture;
    shader_t shader;
    vertex_array_t vertex_array;
    vec2_t resolution;
} framebuffer_t;

framebuffer_t framebuffer_create(const char* vert_file_path, const char* frag_file_path, vec2_t resolution);
void framebuffer_bind(framebuffer_t* framebuffer);
void framebuffer_unbind();
void framebuffer_delete(framebuffer_t* framebuffer);

#define MAX_QUAD_COUNT 10000
#define MAX_TEXTURE_COUNT 16

typedef struct
{
    vec3_t pos;
    vec4_t color;
    vec2_t uv;
    float tex_index;
} quad_vertex_t;

typedef struct 
{
    uint32_t quad_indices[MAX_QUAD_COUNT * 6];
    uint32_t quad_index_count;
    quad_vertex_t quad_vertices[MAX_QUAD_COUNT * 4];
    quad_vertex_t *quad_vertices_p;

    texture_t quad_textures[MAX_TEXTURE_COUNT];
    uint32_t quad_texture_count;

    shader_t quad_shader;
    vertex_array_t quad_vertex_array;
}quad_batch_t;


typedef struct
{
    vec4_t clear_color;

    mat4_t view_mat;
    mat4_t proj_mat;

    uint32_t quad_indices[MAX_QUAD_COUNT * 6];
    uint32_t quad_index_count;
    quad_vertex_t quad_vertices[MAX_QUAD_COUNT * 4];
    quad_vertex_t *quad_vertices_p;

    texture_t quad_textures[MAX_TEXTURE_COUNT];
    uint32_t quad_texture_count;

    shader_t quad_shader;
    vertex_array_t quad_vertex_array;
} renderer_t;

//renderer_t renderer_create(vec4_t clear_color,  vec2_t resolution, bool  )  */
void renderer_init(renderer_t *renderer);


void renderer_start(renderer_t *renderer);
void renderer_end(renderer_t *renderer);

void renderer_draw_elements(renderer_t *renderer, uint32_t index_count);
void renderer_batch_start(renderer_t *renderer);
void renderer_batch_end(renderer_t *renderer);

void renderer_exit(renderer_t *renderer);

void renderer_draw_sub_texture(
    renderer_t *renderer,
    texture_t *texture,
    sub_texture_t *sub_texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color);

void renderer_draw_texture(
    renderer_t *renderer,
    texture_t *texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color);

void renderer_draw_quad(
    renderer_t *renderer,
    vec3_t pos,
    vec3_t size,
    vec4_t color);

typedef struct
{
    const char *title;
    vec2_t size;
    bool closed, minimized, fullscreen, vsync;
    GLFWwindow *glfw;
} window_t;

void window_init(window_t *window);
void window_update(window_t *window);
void window_exit(window_t *window);

typedef enum key
{
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,
    KEY_BACKSLASH = 92,
    KEY_RIGHT_BRACKET = 93,
    KEY_GRAVE_ACCENT = 96,
    KEY_WORLD_1 = 161,
    KEY_WORLD_2 = 162,

    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,
    KEY_F13 = 302,
    KEY_F14 = 303,
    KEY_F15 = 304,
    KEY_F16 = 305,
    KEY_F17 = 306,
    KEY_F18 = 307,
    KEY_F19 = 308,
    KEY_F20 = 309,
    KEY_F21 = 310,
    KEY_F22 = 311,
    KEY_F23 = 312,
    KEY_F24 = 313,
    KEY_F25 = 314,
    KEY_KP_0 = 320,
    KEY_KP_1 = 321,
    KEY_KP_2 = 322,
    KEY_KP_3 = 323,
    KEY_KP_4 = 324,
    KEY_KP_5 = 325,
    KEY_KP_6 = 326,
    KEY_KP_7 = 327,
    KEY_KP_8 = 328,
    KEY_KP_9 = 329,
    KEY_KP_DECIMAL = 330,
    KEY_KP_DIVIDE = 331,
    KEY_KP_MULTIPLY = 332,
    KEY_KP_SUBTRACT = 333,
    KEY_KP_ADD = 334,
    KEY_KP_ENTER = 335,
    KEY_KP_EQUAL = 336,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_MENU = 348
} key;
typedef enum mouse_button
{
    MOUSE_BUTTON_1 = 0,
    MOUSE_BUTTON_2 = 1,
    MOUSE_BUTTON_3 = 2,
    MOUSE_BUTTON_4 = 3,
    MOUSE_BUTTON_5 = 4,
    MOUSE_BUTTON_6 = 5,
    MOUSE_BUTTON_7 = 6,
    MOUSE_BUTTON_8 = 7,
    MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
    MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
    MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
    MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
} mouse_button;

bool key_pressed(window_t *window, key key);
bool mouse_button_pressed(window_t *window, mouse_button button);
vec2_t mouse_position(window_t *window);
