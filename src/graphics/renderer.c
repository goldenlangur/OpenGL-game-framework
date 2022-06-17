#include "graphics/graphics.h"

uint32_t fbo, rbo, fbt, vao, vbo;
shader_t shader;
void renderer_init(renderer_t *renderer)
{
    printf("%u\n", glGetError());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    renderer->quad_index_count = 0;
    renderer->quad_texture_count = 1;

    // pre calculate indices
    int index_offset = 0;
    for (int i = 0; i < MAX_QUAD_COUNT * 6; i += 6)
    {
        renderer->quad_indices[i + 0] = 0 + index_offset;
        renderer->quad_indices[i + 1] = 1 + index_offset;
        renderer->quad_indices[i + 2] = 2 + index_offset;
        renderer->quad_indices[i + 3] = 2 + index_offset;
        renderer->quad_indices[i + 4] = 3 + index_offset;
        renderer->quad_indices[i + 5] = 0 + index_offset;

        index_offset += 4;
    }

    // create vertexarray
    renderer->quad_vertex_array = vertex_array_create(NULL, sizeof(quad_vertex_t) * MAX_QUAD_COUNT * 4, renderer->quad_indices, sizeof(uint32_t) * MAX_QUAD_COUNT * 6, true);
    vertex_array_bind(&renderer->quad_vertex_array);
    vertex_array_push_attribute(0, 3, sizeof(quad_vertex_t), offsetof(quad_vertex_t, pos));
    vertex_array_push_attribute(1, 4, sizeof(quad_vertex_t), offsetof(quad_vertex_t, color));
    vertex_array_push_attribute(2, 2, sizeof(quad_vertex_t), offsetof(quad_vertex_t, uv));
    vertex_array_push_attribute(3, 1, sizeof(quad_vertex_t), offsetof(quad_vertex_t, tex_index));

    // create white texture
    unsigned char white[4] = {255, 255, 255, 255};
    texture_t white_tex = texture_create_from_data(white, (vec2_t){1, 1});
    renderer->quad_textures[0] = white_tex;

    renderer->quad_shader = shader_create("res/shaders/quad_shader.vert", "res/shaders/quad_shader.frag");

    int samplers[MAX_TEXTURE_COUNT];
    for (uint32_t i = 0; i < MAX_TEXTURE_COUNT; i++)
    {
        samplers[i] = i;
    }

    shader_set_uniform_int_arr(&renderer->quad_shader, "u_textures", samplers, MAX_TEXTURE_COUNT);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &fbt);
    glBindTexture(GL_TEXTURE_2D, fbt);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbt, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf(LOG_ERROR "framebuffer error\n");

    float vertices[] = {
        -0.1f, 0.1f, 0.0f, 0.0f, 1.0f,
        -0.1f, -0.1f,  0.0f, 0.0f, 0.0f,
        0.1f, -0.1f,  0.0f,1.0f, 0.0f,

        -0.1f, 0.1f,  0.0f, 0.0f, 1.0f,
        0.1f, -0.1f,  0.0f, 1.0f, 0.0f,
        0.1f, 0.1f,  0.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    shader = shader_create("res/shaders/pp.vert", "res/shaders/pp.frag");
    glUniform1i(glGetUniformLocation(shader.id, "buffer_texture"), 0);
}
void renderer_start(renderer_t *renderer)
{
    if(glGetError() != 0)
        printf("%u\n", glGetError());

    shader_bind(&renderer->quad_shader);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_view_mat", renderer->view_mat);
    shader_set_uniform_mat4(&renderer->quad_shader, "u_proj_mat", renderer->proj_mat);

    glEnable(GL_DEPTH_TEST); 
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_end(renderer_t *renderer)
{
    shader_bind(&shader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); 

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbt); // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderer_batch_start(renderer_t *renderer)
{
    renderer->quad_vertices_p = renderer->quad_vertices;
    renderer->quad_index_count = 0;
    renderer->quad_texture_count = 1;
}
void renderer_draw_elements(renderer_t *renderer, uint32_t index_count)
{
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL);
}
void renderer_batch_end(renderer_t *renderer)
{
    size_t size = (uint8_t *)renderer->quad_vertices_p - (uint8_t *)renderer->quad_vertices;
    vertex_array_bind(&renderer->quad_vertex_array);
    shader_bind(&renderer->quad_shader);
    vertex_array_push_vertex_data(size, renderer->quad_vertices);

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        texture_bind(&renderer->quad_textures[i], i);
    }

    renderer_draw_elements(renderer, renderer->quad_index_count);
}

void renderer_exit(renderer_t *renderer)
{
    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        texture_delete(&renderer->quad_textures[i]);
    }

    shader_delete(&renderer->quad_shader);

    glDeleteBuffers(1, &renderer->quad_vertex_array.vbo);
    glDeleteBuffers(1, &renderer->quad_vertex_array.ibo);
    glDeleteVertexArrays(1, &renderer->quad_vertex_array.vao);
}

void renderer_draw_texture(
    renderer_t *renderer,
    texture_t *texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color)
{
    if (renderer->quad_index_count >= MAX_QUAD_COUNT * 6 || renderer->quad_texture_count >= MAX_TEXTURE_COUNT)
    {
        renderer_batch_end(renderer);
        renderer_batch_start(renderer);
    }
    float tex_index = 0.0f;

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        if (renderer->quad_textures[i].id == texture->id)
        {
            tex_index = (float)i;
            break;
        }
    }

    if (tex_index == 0.0f)
    {
        tex_index = (float)renderer->quad_texture_count;
        renderer->quad_textures[renderer->quad_texture_count] = *texture;
        renderer->quad_texture_count += 1;
    }

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){0.0f, 0.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){1.0f, 0.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){1.0f, 1.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = (vec2_t){0.0f, 1.0f};
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;
    renderer->quad_index_count += 6;
}

void renderer_draw_sub_texture(
    renderer_t *renderer,
    texture_t *texture,
    sub_texture_t *sub_texture,
    vec3_t pos,
    vec3_t size,
    vec4_t color)
{
    if (renderer->quad_index_count >= MAX_QUAD_COUNT * 6 || renderer->quad_texture_count >= MAX_TEXTURE_COUNT)
    {
        renderer_batch_end(renderer);
        renderer_batch_start(renderer);
    }
    float tex_index = 0.0f;

    for (uint32_t i = 0; i < renderer->quad_texture_count; i++)
    {
        if (renderer->quad_textures[i].id == texture->id)
        {
            tex_index = (float)i;
            break;
        }
    }

    if (tex_index == 0.0f)
    {
        tex_index = (float)renderer->quad_texture_count;
        renderer->quad_textures[renderer->quad_texture_count] = *texture;
        renderer->quad_texture_count += 1;
    }

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[0];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[1];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[2];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = sub_texture->uv[3];
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;
    renderer->quad_index_count += 6;
}

void renderer_draw_quad(
    renderer_t *renderer,
    vec3_t pos,
    vec3_t size,
    vec4_t color)
{
    if (renderer->quad_index_count >= MAX_QUAD_COUNT * 6 || renderer->quad_texture_count >= MAX_TEXTURE_COUNT)
    {
        renderer_batch_end(renderer);
        renderer_batch_start(renderer);
    }

    float tex_index = 0.0f;
    vec2_t no_uv = {0.0f, 0.0f};
    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = no_uv;
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = no_uv;
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = no_uv;
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;

    renderer->quad_vertices_p->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->quad_vertices_p->color = color;
    renderer->quad_vertices_p->uv = no_uv;
    renderer->quad_vertices_p->tex_index = tex_index;
    renderer->quad_vertices_p++;
    renderer->quad_index_count += 6;
}
