#include "pch.h"
#include "graphics/graphics.h"

void window_init(window_s* window_p)
{
    window_p->closed = 1;

    if(!glfwInit())
    {
        printf(LOG_ERROR"[OPENGL]: failed to init GLFW!\n");
        exit(-1);
    }
    else
        printf(LOG_INFO"[OPENGL]: initialized GLFW!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_p->glfw = glfwCreateWindow(window_p->size.x, window_p->size.y, window_p->title, NULL, NULL);
    
    glfwMakeContextCurrent(window_p->glfw);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        printf(LOG_ERROR"[OPENGL]: failed to init GLAD!\n");
    else
        printf(LOG_INFO"[OPENGL]: initialized GLAD!\n");

    if(window_p == NULL)
        printf("[ERROR]: Failed to create GLFW window!\n");
    else
        window_p->closed = 0;
    
    glfwSwapInterval(window_p->vsync);
}
void window_update(window_s* window_p)
{
    glfwSwapBuffers(window_p->glfw);
    glfwPollEvents();

    if(glfwWindowShouldClose(window_p->glfw))
        window_p->closed = 1;

    if(window_p->fullscreen)
        glfwMaximizeWindow(window_p->glfw);
}
void window_exit(window_s* window_p)
{
    glfwDestroyWindow(window_p->glfw);
    glfwTerminate();
}

//Input
bool key_pressed(window_s* window_p, key key)
{
    if(glfwGetKey(window_p->glfw, key) == GLFW_PRESS)
        return 1;
    return 0;
}

bool mouse_button_pressed(window_s* window_p, mouse_button button)
{
    if(glfwGetMouseButton(window_p->glfw, button) == GLFW_PRESS)
        return 1;
    return 0;
}

vec2_t mouse_position(window_s* window_p)
{
    vec2_t pos;
    glfwGetCursorPos(window_p->glfw, (double*)&pos.x, (double*)&pos.y);
    return pos;
}