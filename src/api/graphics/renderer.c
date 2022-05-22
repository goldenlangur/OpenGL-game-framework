#include "api/pch.h"
#include "api/graphics/graphics.h"

void renderer_init(renderer_s *renderer)
{
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_MULTISAMPLE);

    renderer->index_count = 0;
    renderer->tex_slot_count = 1;

    renderer->vertex_buffer = malloc(sizeof(vertex_t) * renderer->max_quad_count * 4);
    renderer->index_buffer = malloc(sizeof(uint32_t) * renderer->max_quad_count * 6);

    // pre calculate indices
    int index_offset = 0;
    for (int i = 0; i < renderer->max_quad_count * 6; i += 6)
    {
        renderer->index_buffer[i + 0] = 0 + index_offset;
        renderer->index_buffer[i + 1] = 1 + index_offset;
        renderer->index_buffer[i + 2] = 2 + index_offset;
        renderer->index_buffer[i + 3] = 2 + index_offset;
        renderer->index_buffer[i + 4] = 3 + index_offset;
        renderer->index_buffer[i + 5] = 0 + index_offset;

        index_offset += 4;
    }

    // create vao,vbo and ibo
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * renderer->max_quad_count * 4, NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * renderer->max_quad_count * 6, renderer->index_buffer, GL_STATIC_DRAW);

    // set attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *)offsetof(vertex_t, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *)offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *)offsetof(vertex_t, tex_index));
    glEnableVertexAttribArray(3);

    // create white texture
    texture_t white_tex;
    white_tex.size.x= 1;
    white_tex.size.y = 1;
    unsigned char white[4] = {255, 255, 255, 255};
    white_tex.data = white;
    glGenTextures(1, &white_tex.id);
    glBindTexture(GL_TEXTURE_2D, white_tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, white_tex.size.x, white_tex.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, white_tex.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    renderer->tex_slots[0] = white_tex;

    // load shaders
    const char *v_src = glsl_load_from_file("res/shaders/shader.vert");
    const char *f_src = glsl_load_from_file("res/shaders/shader.frag");

    uint32_t v_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &v_src, NULL);
    glCompileShader(v_shader);

    int succ;
    char log[512];
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(v_shader, 512, NULL, log);
        printf(LOG_ERROR"[shader]: %s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO"[renderer]: compiled vertex shader succesfully!\n");

    uint32_t f_shader;
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &f_src, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(f_shader, 512, NULL, log);
        printf(LOG_ERROR"[shader]:%s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO"[renderer]: compiled fragment shader succesfully!\n");

    renderer->shader = glCreateProgram();
    glAttachShader(renderer->shader, v_shader);
    glAttachShader(renderer->shader, f_shader);
    glLinkProgram(renderer->shader);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(renderer->shader, GL_LINK_STATUS, &succ);
    if (!succ)
    {
        glGetProgramInfoLog(renderer->shader, 512, NULL, log);
        printf(LOG_ERROR"[shader]: %s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO"[renderer]: made shader program succesfully!\n");

    glUseProgram(renderer->shader);

    int samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int loc = glGetUniformLocation(renderer->shader, "u_textures");
    glUniform1iv(loc, 16, samplers);
}
void renderer_start(renderer_s *renderer)
{
    renderer->vertex_buffer_ptr = renderer->vertex_buffer;
    renderer->index_count = 0;
    renderer->tex_slot_count = 1;

    int loc = glGetUniformLocation(renderer->shader, "u_view_mat");
    if (loc == -1)
    {
        printf(LOG_ERROR"[renderer]: no uniform with name %s found!\n", "u_view_mat");
        exit(-1);
    }
    glUniformMatrix4fv(loc, 1, GL_TRUE, &renderer->view_mat.data[0][0]);

    loc = glGetUniformLocation(renderer->shader, "u_proj_mat");
    if (loc == -1)
    {
        printf(LOG_ERROR"[renderer]: no uniform with name %s found!\n", "u_proj_mat");
        exit(-1);
    }
    glUniformMatrix4fv(loc, 1, GL_TRUE, &renderer->proj_mat.data[0][0]);
}
void renderer_end(renderer_s *renderer)
{
    uint32_t size = (uint8_t *)renderer->vertex_buffer_ptr - (uint8_t *)renderer->vertex_buffer;
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer->vertex_buffer);

    glClearColor(renderer->clear_color.x, renderer->clear_color.y, renderer->clear_color.z, renderer->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    for (uint32_t i = 0; i < renderer->tex_slot_count; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderer->tex_slots[i].id);
    }

    glUseProgram(renderer->shader);
    glBindVertexArray(renderer->vao);
    glDrawElements(GL_TRIANGLES, renderer->index_count, GL_UNSIGNED_INT, NULL);
}

void renderer_exit(renderer_s *renderer)
{
    free(renderer->vertex_buffer);
    free(renderer->index_buffer);

    for (uint32_t i = 0; i < renderer->tex_slot_count; i++)
    {
        glDeleteTextures(1, &renderer->tex_slots[i].id);
    }

    glDeleteBuffers(1, &renderer->vbo);
    glDeleteBuffers(1, &renderer->ibo);
    glDeleteVertexArrays(1, &renderer->vao);
}

void renderer_draw_texture(
    renderer_s *renderer,
    texture_t *texture_p,
    sub_texture_t *sub_texture_p,
    const vec3_t pos,
    const vec3_t size,
    const vec4_t color)
{
    float tex_index = 0.0f;

    for (uint32_t i = 0; i < renderer->tex_slot_count; i++)
    {
        if (renderer->tex_slots[i].id == texture_p->id)
        {
            tex_index = (float)i;
            break;
        }
    }

    if (tex_index == 0.0f)
    {
        tex_index = (float)renderer->tex_slot_count;
        renderer->tex_slots[renderer->tex_slot_count] = *texture_p;
        renderer->tex_slot_count += 1;
    }

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = sub_texture_p->uv[0];
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = sub_texture_p->uv[1];
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = sub_texture_p->uv[2];
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = sub_texture_p->uv[3];
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;
    renderer->index_count += 6;
}

void renderer_draw_quad(renderer_s *renderer, const vec3_t pos, const vec3_t size, const vec4_t color)
{
    float tex_index = 0.0f;
    vec2_t no_uv = {0.0f, 0.0f};
    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x - (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = no_uv;
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x + (size.x / 2), pos.y - (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = no_uv;
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x + (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = no_uv;
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;

    renderer->vertex_buffer_ptr->pos = (vec3_t){pos.x - (size.x / 2), pos.y + (size.y / 2), pos.z};
    renderer->vertex_buffer_ptr->color = color;
    renderer->vertex_buffer_ptr->uv = no_uv;
    renderer->vertex_buffer_ptr->tex_index = tex_index;
    renderer->vertex_buffer_ptr++;
    renderer->index_count += 6;
}

const char *glsl_load_from_file(const char *path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
        buffer[length] = '\0';
    }
    else
    {
        printf(LOG_ERROR"[texture]: failed to open file: %s\n", path);
        exit(-1);
    }

    if (buffer)
        return buffer;
    else
    {
        printf(LOG_ERROR"[texture]: failed write file to string!\n");
        exit(-1);
    }
}