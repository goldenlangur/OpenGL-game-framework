#include "api/pch.h"
#include "api/graphics/graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

texture_t texture_create_from_file(const char* path)
{
    texture_t texture;

    stbi_set_flip_vertically_on_load(1);
    int channel_count;
    int x, y;
    texture.data = stbi_load(path, &x, &y, &channel_count, 0);

    texture.size.x = x;
    texture.size.y = y;
    texture.channel_count = channel_count;

    if(texture.data == NULL)
    {
        printf(LOG_ERROR"[texture]: failed to load texture: %s\n", path);
        exit(-1);
    }else
        printf(LOG_INFO"[texture]: Succesfully loaded texture: |%s|\n", path);
    
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture.size.x, texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    free(texture.data);
    return texture;
}

texture_t texture_create_from_data(unsigned char* data, vec2_t size)
{
    if(data == NULL)
    {
        printf(LOG_ERROR"[texture]: data is empty!\n");
        exit(-1);
    }
    texture_t texture;
    texture.size = size;
    texture.data = data;
    texture.channel_count = 4;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture.size.x, texture.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

sub_texture_t sub_texture_create(vec2_t tex_size, vec2_t coords, vec2_t tilesize)
{
    vec2_t min = {(coords.x * tilesize.x) / tex_size.x, (coords.y * tilesize.y) / tex_size.y };
    vec2_t max = {((coords.x + 1) * tilesize.x) / tex_size.x, ((coords.y + 1) * tilesize.y) / tex_size.y };

    sub_texture_t sub_texture;
    sub_texture.uv[0] = (vec2_t){min.x, min.y };
    sub_texture.uv[1] = (vec2_t){max.x, min.y };
    sub_texture.uv[2] = (vec2_t){max.x, max.y};
    sub_texture.uv[3] = (vec2_t){min.x, max.y};
    return sub_texture;
} 