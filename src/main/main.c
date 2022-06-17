#include "api.h"

typedef struct
{
    window_t window;
    renderer_t renderer;
    ecs_t ecs;
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

    do 
    {   
        window_update(&game.window);
        
        renderer_start(&game.renderer);
        renderer_batch_start(&game.renderer);

        game.renderer.proj_mat = mat4_ortho_aspect(1, 1, -10.0f, 10.0f);
        game.renderer.view_mat = mat4_translate(mat4_new(1.0f), (vec3_t){0, 0, 0});
        renderer_draw_quad(&game.renderer, (vec3_t){0.0f , 0.0f}, (vec3_t){0.5f , 0.5f}, (vec4_t){1.0f, 1.0f, 1.0f, 1.0f});
        renderer_batch_end(&game.renderer); 
        
        renderer_end(&game.renderer);

    }while (game.window.closed != 1);

    window_exit(&game.window);
    renderer_exit(&game.renderer);
    
    return 0;
}


/*
last_time = glfwGetTime();
    double last_time = 0;
    double current_time = 0;
   // ecs_init(&game.ecs);
            current_time = glfwGetTime();
        game.fps = 1/(current_time - last_time);
        game.dt = (current_time - last_time);
    //game_loop_init(&game);
          //  game_loop_update(&game); 
       // ecs_update(&game.ecs, &game.window, &game.renderer); 
    //while (game.window.closed != 1);
   // game_loop_exit(&game);
   // ecs_exit(&game.ecs);
window_update(&game.window);
renderer_start(&game.renderer);


game_loop_update(&game);

printf("ECS: %f Renderer: %f\n", game.ecs_time, game.render_time); */