#pragma once

#define MAX_ENTITIES 1000
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
    vec2_t position;
    float rotation;
    vec2_t scale;
} transform_component_s;

typedef struct
{
    int entity_count;
    data_component_s data_components[MAX_ENTITIES + 1];
    transform_component_s transform_components[MAX_ENTITIES + 1];
} ecs_s;

void ecs_reset_entity_values(ecs_s *ecs, entity_t e);
void ecs_init(ecs_s *ecs);
void ecs_update(ecs_s *ecs);
void ecs_exit(ecs_s *ecs);

entity_t ecs_create_entity(ecs_s *ecs, const char *name);
entity_t ecs_delete_entity(ecs_s *ecs, entity_t *entity_t);
