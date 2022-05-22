#include "loop.h"
#include "math.h"

int main(int argc, char const *argv[])
{
    window_s window = (window_s)
    {
        .size = (vec2_t){1400, 700},
        .title = "game",
        .vsync = 1
    };
    window_init(&window);
    
    renderer_s renderer = (renderer_s)
    {
        .clear_color = (vec4_t){0.1f, 0.1f, 0.2f, 255},
        .max_quad_count = 20000
    };
    
    renderer_init(&renderer);

    texture_t texture_sheet = texture_from_file("res/textures/hornet.png"); 
    sub_texture_t frog = sub_texture_create(&texture_sheet, vec2_new(0.0, 1.0), vec2_new(16, 32));
    printf("x: %i y: %i\n", texture_sheet.w, texture_sheet.h);
    while (window.closed != 1)
    {
        float aspect = (float)1400/(float)700;
        
        renderer.proj_mat = mat4_ortho_aspect(aspect, 4, 1000, 0.1f);
        renderer.view_mat = mat4_translate(mat4_new(1.0f), (vec3_t){0.0f, 0.0f, 0.0f});

            
        window_update(&window);
        renderer_start(&renderer);

        
        float speed = 0.1f;
        static vec3_t pos;
        if(key_pressed(&window, KEY_W))
            pos.y+=speed;
        if(key_pressed(&window, KEY_S))
            pos.y-=speed;
        if(key_pressed(&window, KEY_A))
            pos.x-=speed;
        if(key_pressed(&window, KEY_D))
            pos.x+=speed;

        static float time = 0;
        time+=0.1;
        frog = sub_texture_create(&texture_sheet, vec2_new(0, 0), vec2_new(50, 50));
        renderer_draw_quad(&renderer, (vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){1.0f, 1.0f, 1.0f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, (vec3_t){2.0f, 0.0f, 0.0f}, (vec3_t){1.0f, 1.0f, 1.0f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_quad(&renderer, (vec3_t){2.0f, 0.0f, 0.0f}, (vec3_t){1.0f, 1.0f, 1.0f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_draw_texture(&renderer, &texture_sheet, &frog, pos, (vec3_t){1.0f, 2.0f, 1.0f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_end(&renderer);
    }

    window_exit(&window);
    renderer_exit(&renderer);

    return 0;
}
