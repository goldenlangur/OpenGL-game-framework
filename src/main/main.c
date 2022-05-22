#include "api/api.h"

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
        .clear_color = (vec4_t){0.1f, 0.1f, 0.2f, 1.0f},
        .max_quad_count = 200000,
        .proj_mat = mat4_ortho_aspect((float)((float)1400 / (float)700), 2, 1000, 0.1f),
        .view_mat = mat4_translate(mat4_new(1.0f), (vec3_t){0.0f, 0.0f, 0.0f})
    };
    renderer_init(&renderer);

    ecs_s ecs;
    ecs_init(&ecs);
    
    //init game loop

    while (window.closed != 1)
    {
        window_update(&window);
        renderer_start(&renderer);

        //update game loop
            
        ecs_update(&ecs);
        renderer_end(&renderer);
    }

    ecs_exit(&ecs);
    window_exit(&window);
    renderer_exit(&renderer);

    return 0;
}

/* float speed = 0.1f;
static vec3_t pos;
if (key_pressed(&window, KEY_W))
    pos.y += speed;
if (key_pressed(&window, KEY_S))
    pos.y -= speed;
if (key_pressed(&window, KEY_A))
    pos.x -= speed;
if (key_pressed(&window, KEY_D))
    pos.x += speed;

float size = 0.05f;
for (float i = 0; i < 100; i++)
{
    for (float j = 0; j < 100; j++)
    {
        renderer_draw_quad(&renderer, (vec3_t){(i - 50) * size, (j - 50) * size, 0.0f}, (vec3_t){1.0f * size, 1.0f * size, 1.0f * size}, (vec4_t){i / 100, j / 100, 1.0f, 1.0f});
    }
}

renderer_draw_texture(&renderer, &texture_sheet, &frog, pos, (vec3_t){1.0f, 2.0f, 1.0f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});

texture_t texture_sheet = texture_create_from_file("res/textures/hornet.png");
sub_texture_t frog = sub_texture_create(texture_sheet.size, (vec2_t){0.0, 1.0}, (vec2_t){16, 32});
 */