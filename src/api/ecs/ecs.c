#include "api/ecs/ecs.h"

void ecs_init(ecs_s *ecs)
{
    ecs->entity_count = 0;
    for (entity_t e = 0; e <= MAX_ENTITIES; e++)
    {
        ecs_reset_entity_values(ecs, e);
    }
}

void ecs_reset_entity_values(ecs_s *ecs, entity_t e)
{  
    ecs->data_components[e] = (data_component_s){
        .name = NULL,
        .type = NULL,
        .layer = 0,
    };
    ecs->transform_components[e] = (transform_component_s){
        .position = (vec2_t){0.0f, 0.0f},
        .scale = (vec2_t){0.0f, 0.0f},
        .rotation = 0.0f}; 
}

entity_t ecs_create_entity(ecs_s *ecs, const char *name)
{
    unsigned int e = 0;

    if (ecs->entity_count != MAX_ENTITIES)
    {
        ecs->entity_count += 1;
        e = ecs->entity_count;
    }
    else
    {
        for (entity_t i = 1; i <= MAX_ENTITIES + 1; i++)
        {
            if (ecs->data_components[i].name == NULL)
                e = i;
        }
        if (e == 0)
        {
            printf("[ERROR]: Maximum amount of entities reached!\n");
        }
    }
    ecs->data_components[e].name = name;
    return e;
}

entity_t ecs_delete_entity(ecs_s *ecs, entity_t *entity_t)
{
    ecs_reset_entity_values(ecs, *entity_t);
    ecs->entity_count -= 1;
    return 0;
}

void ecs_update(ecs_s *ecs)
{
    for (unsigned int i = 1; i <= ecs->entity_count; i++)
    {
        
    }
}

void ecs_exit(ecs_s *ecs)
{
    for (entity_t e = 0; e <= MAX_ENTITIES + 1; e++)
    {
        ecs_reset_entity_values(ecs, e);
    } 
}

/*

    ecs_init(&ecs);

    player_entity = ecs_create_entity(&ecs, "player");
    ecs.transform_components[player_entity].position = (vec2_t){2, 4};
    ecs.transform_components[player_entity].position.x += 0.002;

    ecs_update(&ecs);

    player_entity = ecs_delete_entity(&ecs, &player_entity);

 */