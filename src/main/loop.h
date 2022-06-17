#pragma once

#include "api.h"

entity_t player;
entity_t camera;

entity_t noise_entity;

texture_t texture_map;
sub_texture_t sub_texture_1;
sub_texture_t sub_texture_2;

texture_t noise_texture;

#define SIZE 128
unsigned char noise_texture_data[SIZE * SIZE * 4];
struct component
{
};

void game_loop_init(game_s *game)
{
    texture_map = texture_create_from_file("res/textures/hornet.png");
    sub_texture_1 = sub_texture_create((vec2_t){64, 64}, (vec2_t){0, 1}, (vec2_t){16, 32});

    player = ecs_create_entity(&game->ecs, "player");
    game->ecs.transform_components[player].scale = (vec3_t){1.0f, 2.0f, 1.0f};
    game->ecs.transform_components[player].position.z = 1;
    game->ecs.transform_components[player].position.x = 2;
    game->ecs.sprite_components[player].active = true;

    game->ecs.sprite_components[player].texture = &texture_map;
    game->ecs.sprite_components[player].sub_texture = &sub_texture_1; 

    camera = ecs_create_entity(&game->ecs, "camera");
    game->ecs.camera_components[camera].active = true;
    game->ecs.camera_components[camera].ortho_size = 32.0f;

    noise_entity = ecs_create_entity(&game->ecs, "noise");
    game->ecs.sprite_components[noise_entity].active = true;
    game->ecs.transform_components[noise_entity].scale = (vec3_t){8.0f, 8.0f, 1.0f};
    game->ecs.transform_components[noise_entity].position.z = 0;
    game->ecs.sprite_components[noise_entity].texture = &noise_texture; 
}

void game_loop_update(game_s *game)
{
    /* static int count = 0;
    count++;
    if (count > 10)
        count = 0;
    if (count == 0)
    {
        for (int i = 0; i < SIZE * SIZE * 4; i += 4)
        {
            int x = rand() % 255;
            noise_texture_data[i + 0] = x;
            noise_texture_data[i + 1] = x;
            noise_texture_data[i + 2] = x;
            noise_texture_data[i + 3] = 255;
        }
    }
    texture_update_data(&noise_texture, noise_texture_data, noise_texture.size); */

    /* float speed = 10.0 * game->dt;
    if(key_pressed(&game->window, KEY_D))
    {
        game->ecs.transform_components[player].position.x += speed;
        game->ecs.transform_components[player].scale.x = fabs(game->ecs.transform_components[player].scale.x);
    }
    if(key_pressed(&game->window, KEY_A))
    {
        game->ecs.transform_components[player].position.x -= speed;
        game->ecs.transform_components[player].scale.x = -fabs(game->ecs.transform_components[player].scale.x);
    }
    if(key_pressed(&game->window, KEY_W))
    {
        game->ecs.transform_components[player].position.y += speed;
    }
    if(key_pressed(&game->window, KEY_S))
    {
        game->ecs.transform_components[player].position.y -= speed;
    }

    if (key_pressed(&game->window, KEY_SPACE))
        game->ecs.camera_components[camera].ortho_size -= 10.f * game->ecs.camera_components[camera].ortho_size *game->dt;
    if (key_pressed(&game->window, KEY_LEFT_SHIFT))
        game->ecs.camera_components[camera].ortho_size += 10.f * game->ecs.camera_components[camera].ortho_size *game->dt;
 
    static int fps_time = 0;
    fps_time++;
    if (fps_time > game->fps)
    {
        fps_time = 0;
        printf("FPS: %f - mili: %f\n", game->fps, 1.0f/game->fps);
    } */
}
void game_loop_exit(game_s *game)
{
}
