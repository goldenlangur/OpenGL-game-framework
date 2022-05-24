#pragma once
#include "graphics/graphics.h"

#define MAX_ENTITIES 2
// Entity Component System
typedef unsigned int entity_t;

typedef struct
{
    const char *name;
    const char *type;
    int layer;
} data_component_s;

typedef struct
{
    vec3_t position;
    vec3_t scale;
    float rotation;
} transform_component_s;

typedef struct 
{
    bool active;
    float ortho_size;
}camera_component_s;

typedef struct
{
    bool active;
    texture_t* texture;
    sub_texture_t* sub_texture; 
    vec4_t tint_color;
}sprite_component_s;

typedef struct
{
    bool active;
    texture_t* tile_map;
    int** map;
    vec2_t map_size;
}tilemap_component_s;

typedef struct
{
    int entity_count;
    data_component_s data_components[MAX_ENTITIES + 1];
    transform_component_s transform_components[MAX_ENTITIES + 1];
    sprite_component_s sprite_components[MAX_ENTITIES + 1];
    camera_component_s camera_components[MAX_ENTITIES + 1];
} ecs_s;

void ecs_reset_entity_values(ecs_s *ecs, entity_t e);
void ecs_init(ecs_s *ecs);
void ecs_update(ecs_s *ecs, window_s* window, renderer_s* renderer);
void ecs_exit(ecs_s *ecs);

entity_t ecs_create_entity(ecs_s *ecs, const char *name);
void ecs_delete_entity(ecs_s *ecs, entity_t *entity);
