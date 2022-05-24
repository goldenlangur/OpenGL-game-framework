#pragma once

#include "api.h"

entity_t cube;
entity_t camera;

texture_t texture_map;
sub_texture_t sub_texture_1;

void game_loop_init(game_s* game)
{
    texture_map = texture_create_from_file("res/textures/hornet.png");
    sub_texture_1 = sub_texture_create((vec2_t){64, 64}, (vec2_t){0, 1}, (vec2_t){16, 32});

    cube = ecs_create_entity(&game->ecs, "cube");
    game->ecs.transform_components[cube].scale = (vec3_t){1.0f, 2.0f, 1.0f};
    game->ecs.sprite_components[cube].active = true;
    
    game->ecs.sprite_components[cube].texture = &texture_map;
    game->ecs.sprite_components[cube].sub_texture = &sub_texture_1;

    camera = ecs_create_entity(&game->ecs, "camera");
    game->ecs.camera_components[camera].active = true;
    game->ecs.camera_components[camera].ortho_size = 10.0f;
    
}
void game_loop_update(game_s* game)
{
    game->ecs.transform_components[cube].position.x += (float)key_pressed(&game->window, KEY_D)/10;
    game->ecs.transform_components[cube].position.x -= (float)key_pressed(&game->window, KEY_A)/10;
    game->ecs.transform_components[cube].position.y += (float)key_pressed(&game->window, KEY_W)/10;
    game->ecs.transform_components[cube].position.y -= (float)key_pressed(&game->window, KEY_S)/10;

}
void game_loop_exit(game_s* game)
{

}
