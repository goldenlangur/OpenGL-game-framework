#include "graphics/graphics.h"

shader_t shader_create(const char *vert_file_path, const char *frag_file_path)
{
    // load shaders
    shader_t shader;
    shader.v_src = glsl_load_from_file(vert_file_path);
    shader.f_src = glsl_load_from_file(frag_file_path);

    uint32_t v_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &shader.v_src, NULL);
    glCompileShader(v_shader);

    int succ;
    char log[512];
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(v_shader, 512, NULL, log);
        printf(LOG_ERROR "[shader]: %s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO "[renderer]: compiled vertex shader succesfully!\n");

    uint32_t f_shader;
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &shader.f_src, NULL);
    glCompileShader(f_shader);

    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &succ);
    if (!succ)
    {
        glGetShaderInfoLog(f_shader, 512, NULL, log);
        printf(LOG_ERROR "[shader]:%s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO "[renderer]: compiled fragment shader succesfully!\n");

    shader.id = glCreateProgram();
    glAttachShader(shader.id, v_shader);
    glAttachShader(shader.id, f_shader);
    glLinkProgram(shader.id);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    glGetProgramiv(shader.id, GL_LINK_STATUS, &succ);
    if (!succ)
    {
        glGetProgramInfoLog(shader.id, 512, NULL, log);
        printf(LOG_ERROR "[shader]: %s\n", log);
        exit(-1);
    }
    else
        printf(LOG_INFO "[renderer]: made shader program succesfully!\n");
    glUseProgram(shader.id);
    return shader;
}
void shader_bind(shader_t *shader)
{
    glUseProgram(shader->id);
}
void shader_delete(shader_t *shader)
{
    glDeleteProgram(shader->id);
}

void shader_set_uniform_mat4(shader_t *shader, const char *name, mat4_t matrix)
{
    int loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glUniformMatrix4fv(loc, 1, GL_TRUE, &matrix.data[0][0]);
}

void shader_set_uniform_int_arr(shader_t *shader, const char *name, int *data, uint32_t count)
{
    int loc = glGetUniformLocation(shader->id, name);
    if (loc == -1)
    {
        printf(LOG_ERROR "[shader]: no uniform with name %s found!\n", name);
        exit(-1);
    }
    glUniform1iv(loc, count, data);
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
        printf(LOG_ERROR "[texture]: failed to open file: %s\n", path);
        exit(-1);
    }

    if (buffer)
        return buffer;
    else
    {
        printf(LOG_ERROR "[texture]: failed write file to string!\n");
        exit(-1);
    }
}