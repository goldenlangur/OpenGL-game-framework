#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "api/graphics/graphics.h"

void renderer_init(renderer_s *renderer_p)
{
    //glEnable(GL_DEPTH_TEST);  
    //glDepthFunc(GL_LESS);    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    //glEnable(GL_MULTISAMPLE); 

    renderer_p->index_count = 0;
    renderer_p->tex_slot_count = 1;

    renderer_p->vertex_buffer = malloc(sizeof(vertex_t) * renderer_p->max_quad_count * 4);
    renderer_p->index_buffer = malloc(sizeof(uint32_t) * renderer_p->max_quad_count * 6);

    int index_offset = 0;
    for (int i = 0; i < renderer_p->max_quad_count * 6; i+=6)
    {
        renderer_p->index_buffer[i +  0] = 0 + index_offset;
        renderer_p->index_buffer[i +  1] = 1 + index_offset;
        renderer_p->index_buffer[i +  2] = 2 + index_offset;
        renderer_p->index_buffer[i +  3] = 2 + index_offset;
        renderer_p->index_buffer[i +  4] = 3 + index_offset;
        renderer_p->index_buffer[i +  5] = 0 + index_offset;
        
        index_offset+=4;
    }

    glGenVertexArrays(1, &renderer_p->vao); 
    glBindVertexArray(renderer_p->vao);

    glGenBuffers(1, &renderer_p->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_p->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * renderer_p->max_quad_count * 4, NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer_p->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_p->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * renderer_p->max_quad_count * 6 , renderer_p->index_buffer,  GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(vertex_t), (void*)offsetof(vertex_t, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, uv));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, tex_index));
    glEnableVertexAttribArray(3);

    texture_t white_tex;
    white_tex.w = 1;
    white_tex.h = 1;
    unsigned char white[4] = {255,255,255,255};
    white_tex.data = white; 
    glGenTextures(1, &white_tex.id);
    glBindTexture(GL_TEXTURE_2D, white_tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, white_tex.w, white_tex.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, white_tex.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    renderer_p->tex_slots[0] = white_tex;
    
    renderer_p->shader.v_src = glsl_load_from_file("res/shaders/shader.vert");
    renderer_p->shader.f_src = glsl_load_from_file("res/shaders/shader.frag");

    uint32_t v_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &renderer_p->shader.v_src, NULL);
    glCompileShader(v_shader);

    int succ;
    char log[512];
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &succ);
    if(!succ)
    {
        glGetShaderInfoLog(v_shader, 512, NULL, log);
        printf("[ERROR]: %s\n", log);
    }else
        printf("[INFO]: compiled vertex shader succesfully!\n");

    uint32_t f_shader;
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &renderer_p->shader.f_src, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &succ);
    if(!succ)
    {
        glGetShaderInfoLog(f_shader, 512, NULL, log);
        printf("[ERROR]: %s\n", log);
    }else
        printf("[INFO]: compiled fragment shader succesfully!\n");

    renderer_p->shader.id = glCreateProgram();
    glAttachShader(renderer_p->shader.id, v_shader);
    glAttachShader(renderer_p->shader.id, f_shader);
    glLinkProgram(renderer_p->shader.id);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(renderer_p->shader.id, GL_LINK_STATUS, &succ);
    if(!succ) 
    {
        glGetProgramInfoLog(renderer_p->shader.id, 512, NULL, log);
        printf("[ERROR]: %s\n", log);
    }else
        printf("[INFO]: made shader program succesfully!\n");

    glUseProgram(renderer_p->shader.id);

    int samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int loc = glGetUniformLocation(renderer_p->shader.id, "u_textures");
    glUniform1iv(loc, 16, samplers);
}
void renderer_start(renderer_s* renderer_p)
{
    renderer_p->vertex_buffer_ptr = renderer_p->vertex_buffer;
    renderer_p->index_count = 0;
    renderer_p->tex_slot_count = 1;

    int loc = glGetUniformLocation(renderer_p->shader.id, "u_view_mat");
    if(loc == -1)
        printf("Shader: no uniform with name %s found!\n", "u_view_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &renderer_p->view_mat.data[0][0]);

    loc = glGetUniformLocation(renderer_p->shader.id, "u_proj_mat");
    if(loc == -1)
        printf("Shader: no uniform with name %s found!\n", "u_proj_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &renderer_p->proj_mat.data[0][0]);
}
void renderer_end(renderer_s* renderer_p)
{   
    uint32_t size = (uint8_t*)renderer_p->vertex_buffer_ptr - (uint8_t*)renderer_p->vertex_buffer;
    glBindBuffer(GL_ARRAY_BUFFER, renderer_p->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer_p->vertex_buffer);

    glClearColor(renderer_p->clear_color.x, renderer_p->clear_color.y, renderer_p->clear_color.z, renderer_p->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    for (uint32_t i = 0; i < renderer_p->tex_slot_count; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, renderer_p->tex_slots[i].id);
    }

    glUseProgram(renderer_p->shader.id);
    glBindVertexArray(renderer_p->vao);
    glDrawElements(GL_TRIANGLES, renderer_p->index_count, GL_UNSIGNED_INT, NULL);
}

void renderer_exit(renderer_s *renderer_p)
{
    free(renderer_p->vertex_buffer);
    free(renderer_p->index_buffer);

    glDeleteBuffers(1, &renderer_p->vbo);
    glDeleteBuffers(1, &renderer_p->ibo);
    glDeleteVertexArrays(1, &renderer_p->vao);
}

void renderer_draw_texture(
    renderer_s* renderer_p, 
    const texture_t* texture_p, 
     sub_texture_t* sub_texture_p, 
    const vec3_t pos, 
    const vec3_t size,
    const vec4_t color)
{
     float tex_index = 0.0f;
     
     for (uint32_t i = 0; i < renderer_p->tex_slot_count; i++)
    {
        if(renderer_p->tex_slots[i].id == texture_p->id)
        {
            tex_index = (float)i;
            break;
        }
    }
    
    if(tex_index == 0.0f)
    {
        tex_index = (float)renderer_p->tex_slot_count;
        renderer_p->tex_slots[renderer_p->tex_slot_count] = *texture_p;
        renderer_p->tex_slot_count+=1;
    }    

    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x - (size.x/2), pos.y - (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = sub_texture_p->uv[0];;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;

    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x + (size.x/2), pos.y - (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = sub_texture_p->uv[1];;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;
     
    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x + (size.x/2), pos.y + (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = sub_texture_p->uv[2];;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;

    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x - (size.x/2), pos.y + (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = sub_texture_p->uv[3];;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;
    renderer_p->index_count+=6;
}

void renderer_draw_quad(renderer_s* renderer_p, const vec3_t pos, const vec3_t size, const vec4_t color)
{
    //white texture
    float tex_index = 0.0f;
    vec2_t no_uv = {0.0f, 0.0f};
    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x - (size.x/2), pos.y - (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = no_uv;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;

    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x + (size.x/2), pos.y - (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = no_uv;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;
     
    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x + (size.x/2), pos.y + (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = no_uv;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;

    renderer_p->vertex_buffer_ptr->pos = vec3_new(pos.x - (size.x/2), pos.y + (size.y/2), pos.z);
    renderer_p->vertex_buffer_ptr->color = color;
    renderer_p->vertex_buffer_ptr->uv = no_uv;
    renderer_p->vertex_buffer_ptr->tex_index = tex_index;
    renderer_p->vertex_buffer_ptr++;
    renderer_p->index_count+=6;
}

void renderer_set_view_proj(renderer_s* renderer_p, const mat4_t view, const mat4_t proj)
{
    int loc = glGetUniformLocation(renderer_p->shader.id, "u_view_mat");
    if(loc == -1)
        printf("Shader: no uniform with name %s found!\n", "u_view_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &view.data[0][0]);

    loc = glGetUniformLocation(renderer_p->shader.id, "u_proj_mat");
    if(loc == -1)
        printf("Shader: no uniform with name %s found!\n", "u_proj_mat");
    glUniformMatrix4fv(loc, 1, GL_TRUE, &proj.data[0][0]);
}

texture_t texture_from_file(const char* path)
{
    texture_t texture;

    stbi_set_flip_vertically_on_load(1);
    int channel_count;
    texture.data = stbi_load(path, &texture.w, &texture.h, &channel_count, 0);

    if(texture.data == NULL)
    {
        printf("Failed to load texture: %s\n", path);
        exit(-1);
    }else
        printf("[INFO]: Succesfully loaded texture from: %s\n", path);
    
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    
    free(texture.data);
    return texture;
}

void texture_destroy(texture_t* texture)
{
    //glDeleteTextures(1, &texture->id);
}

sub_texture_t sub_texture_create(texture_t* texture, vec2_t coords, vec2_t tilesize)
{
    vec2_t min = {(coords.x * tilesize.x) / texture->w, (coords.y * tilesize.y) / texture->h };
    vec2_t max = {((coords.x + 1) * tilesize.x) / texture->w, ((coords.y + 1) * tilesize.y) / texture->h };

    sub_texture_t sub_texture;

    sub_texture.uv[0] = (vec2_t){0.0f, 0.5f};
    sub_texture.uv[1] = (vec2_t){0.25f, 0.5f};
    sub_texture.uv[2] = (vec2_t){0.25f, 1.0f};
    sub_texture.uv[3] = (vec2_t){0.0f, 1.0f};
    return sub_texture;
} 

const char* glsl_load_from_file(const char* path)
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
    }else
    {
        printf("Failed to open file: %s\n", path);
        exit(-1);
    }
    
    if (buffer)  
        return buffer; 
    else 
    {
        printf("Failed write file to string!\n"); 
        exit(-1);
    }
}