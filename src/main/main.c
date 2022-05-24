#include "api.h"

typedef struct
{
    window_s window;
    renderer_s renderer;
    ecs_s ecs;

    float dt;
} game_s;

#include "main/loop.h"

int main(int argc, char const *argv[])
{
    game_s game;
    game.window = (window_s){
        .size = (vec2_t){800, 800},
        .title = "game",
        .vsync = 1};
    window_init(&game.window);

    game.renderer = (renderer_s){
        .clear_color = (vec4_t){0.2f, .2f, .2f, 1.0f},
        .max_quad_count = 200000,
        .proj_mat = mat4_ortho_aspect((float)((float)game.window.size.x / (float)game.window.size.y), 1, 1000, 0.1f),
        .view_mat = mat4_translate(mat4_new(1.0f), (vec3_t){0.0f, 0.0f, 0.0f})};
    renderer_init(&game.renderer);
    ecs_init(&game.ecs);

    game_loop_init(&game);

    double last_time;
    double current_time = 0;

    while (game.window.closed != 1)
    {
        
        window_update(&game.window);
        renderer_start(&game.renderer);
        game_loop_update(&game);
        ecs_update(&game.ecs, &game.window, &game.renderer);
        renderer_end(&game.renderer); 
    }
    game_loop_exit(&game);
    ecs_exit(&game.ecs);
    window_exit(&game.window);
    renderer_exit(&game.renderer);

    return 0;
}

/*
window_update(&game.window);
renderer_start(&game.renderer);


game_loop_update(&game);

last_time = glfwGetTime();
ecs_update(&game.ecs, &game.window, &game.renderer);
current_time = glfwGetTime();
game.ecs_time = (current_time - last_time)*1000;
last_time = current_time;
renderer_end(&game.renderer);
current_time = glfwGetTime();
game.render_time = (current_time - last_time)*1000;

printf("ECS: %f Renderer: %f\n", game.ecs_time, game.render_time); */