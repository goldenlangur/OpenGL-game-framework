#include "pch.h"
#include "graphics/graphics.h"


void window_init(window_t* window)
{
    window->closed = 1;

    if(!glfwInit())
    {
        printf(LOG_ERROR"[OPENGL]: failed to init GLFW!\n");
        exit(-1);
    }
    else
        printf(LOG_INFO"[OPENGL]: initialized GLFW!\n");
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window->glfw = glfwCreateWindow(window->size.x, window->size.y, window->title, NULL, NULL);
    
    glfwMakeContextCurrent(window->glfw);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        printf(LOG_ERROR"[OPENGL]: failed to init GLAD!\n");
    else
        printf(LOG_INFO"[OPENGL]: initialized GLAD!\n");

    if(window == NULL)
        printf("[ERROR]: Failed to create GLFW window!\n");
    else
        window->closed = 0;
    window->vsync = 1 ;
    window->fullscreen = 0;
    window->minimized = 0;
    glfwSwapInterval(window->vsync);
    
}
void window_update(window_t* window)
{
    glfwSwapBuffers(window->glfw);
    glfwPollEvents();

    if(glfwWindowShouldClose(window->glfw))
        window->closed = 1;
    glfwGetWindowSize(window->glfw, (int*)&window->size.x,  (int*)&window->size.y);
}
void window_exit(window_t* window)
{
    glfwDestroyWindow(window->glfw);
    glfwTerminate();
}

//Input
bool key_pressed(window_t* window, key key)
{
    if(glfwGetKey(window->glfw, key) == GLFW_PRESS)
        return 1;
    return 0;
}

bool mouse_button_pressed(window_t* window, mouse_button button)
{
    if(glfwGetMouseButton(window->glfw, button) == GLFW_PRESS)
        return 1;
    return 0;
}

vec2_t mouse_position(window_t* window)
{
    vec2_t pos;
    glfwGetCursorPos(window->glfw, (double*)&pos.x, (double*)&pos.y);
    return pos;
}