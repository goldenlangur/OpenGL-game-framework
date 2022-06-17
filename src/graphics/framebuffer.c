#include "graphics/graphics.h"

framebuffer_t framebuffer_create(const char *vert_file_path, const char *frag_file_path, vec2_t resolution)
{
    framebuffer_t framebuffer;
    framebuffer.resolution = resolution;

    glGenFramebuffers(1, &framebuffer.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);

    glGenTextures(1, &framebuffer.texture.id);
    glBindTexture(GL_TEXTURE_2D, framebuffer.texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.texture.id, 0);

    glGenRenderbuffers(1, &framebuffer.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        printf(LOG_INFO "[framebuffer]: Successfully created framebuffer!\n");
    }
    else
    {
        printf(LOG_INFO "[framebuffer]: failed to create framebuffer!\n");
        exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float vertices[] =
        {
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    uint32_t indices[] =
        {
            0, 1, 3,
            1, 2, 3};

    framebuffer.vertex_array = vertex_array_create(vertices, sizeof(float) * 20, indices, sizeof(uint32_t) * 6, true);
    vertex_array_bind(&framebuffer.vertex_array);
    vertex_array_push_attribute(0, 3, 5, 0);
    vertex_array_push_attribute(1, 2, 5, 3 * sizeof(float));

    framebuffer.shader = shader_create(vert_file_path, frag_file_path);
    printf("%u\n", framebuffer.shader.id);
    shader_set_uniform_int(&framebuffer.shader, "buffer_texture", 0);

    return framebuffer;
}
void framebuffer_bind(framebuffer_t *framebuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
}
void framebuffer_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebuffer_delete(framebuffer_t *framebuffer)
{
    glDeleteFramebuffers(1, &framebuffer->fbo);
}