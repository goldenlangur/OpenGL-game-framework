#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "api/math/math.h"

typedef struct
{
    const char* v_src;
    const char* f_src;
    uint32_t id;
}shader_s;

typedef struct
{
    uint32_t id;
    int w, h;
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

    shader_s shader;

    uint32_t* index_buffer;
    uint32_t index_count;

    vertex_t* vertex_buffer;
    vertex_t* vertex_buffer_ptr;

    texture_t tex_slots[16];
    uint32_t tex_slot_count;
}renderer_s;


void renderer_init(renderer_s* renderer_p);
void renderer_start(renderer_s* renderer_p);
void renderer_end(renderer_s* renderer_p);
void renderer_exit(renderer_s* renderer_p);

void renderer_draw_texture(
    renderer_s* renderer_p, 
    const texture_t* texture_p, 
     sub_texture_t* sub_texture_p, 
    const vec3_t pos, 
    const vec3_t size,
    const vec4_t color);

void renderer_draw_quad(
    renderer_s* renderer_p, 
    const vec3_t pos, 
    const vec3_t size, 
    const vec4_t color);

void renderer_set_view_proj(renderer_s* renderer_p, const mat4_t view, const mat4_t proj);

texture_t texture_from_file(const char* path);
void texture_destroy(texture_t* texture);

sub_texture_t sub_texture_create(texture_t* texture, vec2_t coords, vec2_t tilesize);

const char* glsl_load_from_file(const char* path);

typedef struct 
{
    const char* title;
    vec2_t size;
    int closed, minimized, fullscreen, vsync;
    GLFWwindow* glfw;
}window_s;

void window_init(window_s* window_p);
void window_update(window_s* window_p);
void window_exit(window_s* window_p);

typedef enum key
{
    KEY_SPACE            =  32,
    KEY_APOSTROPHE       =  39,  
    KEY_COMMA            =  44,  
    KEY_MINUS            =  45,  
    KEY_PERIOD           =  46,  
    KEY_SLASH            =  47,  
    KEY_0                =  48,
    KEY_1                =  49,
    KEY_2                =  50,
    KEY_3                =  51,
    KEY_4                =  52,
    KEY_5                =  53,
    KEY_6                =  54,
    KEY_7                =  55,
    KEY_8                =  56,
    KEY_9                =  57,
    KEY_SEMICOLON        =  59,  
    KEY_EQUAL            =  61,  
    KEY_A                =  65,
    KEY_B                =  66,
    KEY_C                =  67,
    KEY_D                =  68,
    KEY_E                =  69,
    KEY_F                =  70,
    KEY_G                =  71,
    KEY_H                =  72,
    KEY_I                =  73,
    KEY_J                =  74,
    KEY_K                =  75,
    KEY_L                =  76,
    KEY_M                =  77,
    KEY_N                =  78,
    KEY_O                =  79,
    KEY_P                =  80,
    KEY_Q                =  81,
    KEY_R                =  82,
    KEY_S                =  83,
    KEY_T                =  84,
    KEY_U                =  85,
    KEY_V                =  86,
    KEY_W                =  87,
    KEY_X                =  88,
    KEY_Y                =  89,
    KEY_Z                =  90,
    KEY_LEFT_BRACKET     =  91,  /* [ */
    KEY_BACKSLASH        =  92,  /* \ */
    KEY_RIGHT_BRACKET    =  93,  /* ] */
    KEY_GRAVE_ACCENT     =  96,  /* ` */
    KEY_WORLD_1          =  161, /* non-US #1 */
    KEY_WORLD_2          =  162, /* non-US #2 */

    KEY_ESCAPE           =  256,
    KEY_ENTER            =  257,
    KEY_TAB              =  258,
    KEY_BACKSPACE        =  259,
    KEY_INSERT           =  260,
    KEY_DELETE           =  261,
    KEY_RIGHT            =  262,
    KEY_LEFT             =  263,
    KEY_DOWN             =  264,
    KEY_UP               =  265,
    KEY_PAGE_UP          =  266,
    KEY_PAGE_DOWN        =  267,
    KEY_HOME             =  268,
    KEY_END              =  269,
    KEY_CAPS_LOCK        =  280,
    KEY_SCROLL_LOCK      =  281,
    KEY_NUM_LOCK         =  282,
    KEY_PRINT_SCREEN     =  283,
    KEY_PAUSE            =  284,
    KEY_F1               =  290,
    KEY_F2               =  291,
    KEY_F3               =  292,
    KEY_F4               =  293,
    KEY_F5               =  294,
    KEY_F6               =  295,
    KEY_F7               =  296,
    KEY_F8               =  297,
    KEY_F9               =  298,
    KEY_F10              =  299,
    KEY_F11              =  300,
    KEY_F12              =  301,
    KEY_F13              =  302,
    KEY_F14              =  303,
    KEY_F15              =  304,
    KEY_F16              =  305,
    KEY_F17              =  306,
    KEY_F18              =  307,
    KEY_F19              =  308,
    KEY_F20              =  309,
    KEY_F21              =  310,
    KEY_F22              =  311,
    KEY_F23              =  312,
    KEY_F24              =  313,
    KEY_F25              =  314,
    KEY_KP_0             =  320,
    KEY_KP_1             =  321,
    KEY_KP_2             =  322,
    KEY_KP_3             =  323,
    KEY_KP_4             =  324,
    KEY_KP_5             =  325,
    KEY_KP_6             =  326,
    KEY_KP_7             =  327,
    KEY_KP_8             =  328,
    KEY_KP_9             =  329,
    KEY_KP_DECIMAL       =  330,
    KEY_KP_DIVIDE        =  331,
    KEY_KP_MULTIPLY      =  332,
    KEY_KP_SUBTRACT      =  333,
    KEY_KP_ADD           =  334,
    KEY_KP_ENTER         =  335,
    KEY_KP_EQUAL         =  336,
    KEY_LEFT_SHIFT       =  340,
    KEY_LEFT_CONTROL     =  341,
    KEY_LEFT_ALT         =  342,
    KEY_LEFT_SUPER       =  343,
    KEY_RIGHT_SHIFT      =  344,
    KEY_RIGHT_CONTROL    =  345,
    KEY_RIGHT_ALT        =  346,
    KEY_RIGHT_SUPER      =  347,
    KEY_MENU             =  348    
}key;
typedef enum mouse_button
{
    MOUSE_BUTTON_1       =  0,
    MOUSE_BUTTON_2       =  1,
    MOUSE_BUTTON_3       =  2,
    MOUSE_BUTTON_4       =  3,
    MOUSE_BUTTON_5       =  4,
    MOUSE_BUTTON_6       =  5,
    MOUSE_BUTTON_7       =  6,
    MOUSE_BUTTON_8       =  7,
    MOUSE_BUTTON_LAST    =  MOUSE_BUTTON_8,
    MOUSE_BUTTON_LEFT    =  MOUSE_BUTTON_1,
    MOUSE_BUTTON_RIGHT   =  MOUSE_BUTTON_2,
    MOUSE_BUTTON_MIDDLE  =  MOUSE_BUTTON_3
}mouse_button;

int key_pressed(window_s* window_p, key key);
int mouse_button_pressed(window_s* window_p, mouse_button button);
vec2_t mouse_position(window_s* window_p);
