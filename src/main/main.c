#include "api.h"

typedef struct
{
    window_t window;
    renderer_t renderer;
    ecs_t ecs;

    float dt;
    float fps;
} game_s;

#include "main/loop.h"

int main()
{
    game_s game;
    game.window = (window_t)
    {
        .size = (vec2_t){800, 800},
        .title = "game"
    };
    window_init(&game.window);

    game.renderer = (renderer_t)
    {
        .clear_color = (vec4_t){.0f, .0f, .0f, 1.0f}
    };
    renderer_init(&game.renderer);
    ecs_init(&game.ecs);

    game_loop_init(&game);

    double last_time = 0;
    double current_time = 0;
    
    while (game.window.closed != 1)
    {   
        last_time = glfwGetTime();
        window_update(&game.window);
        renderer_clear(&game.renderer);
        game_loop_update(&game);
        renderer_batch_start(&game.renderer);
        ecs_update(&game.ecs, &game.window, &game.renderer);
        renderer_batch_end(&game.renderer); 
        current_time = glfwGetTime();
        game.fps = 1/(current_time - last_time);
        game.dt = (current_time - last_time);
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

printf("ECS: %f Renderer: %f\n", game.ecs_time, game.render_time); */