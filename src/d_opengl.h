#ifndef DT25_D_OPENGL_H
#define DT25_D_OPENGL_H

#include "d_global.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int viewportWidth, viewportHeight;

GLFWwindow* d_setupGLFW() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    viewportWidth = 800;
    viewportHeight = 600;
    auto d_Window = glfwCreateWindow(viewportWidth, viewportHeight, "hillaire-benchmark-reevaluation", NULL, NULL);
    if (d_Window == NULL)
    {
        d_Logger::err("Failed to create GLFW window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(d_Window);
    if (glewInit() != GLEW_OK) {
        d_Logger::err("Failed to init a GLEW.");
        return nullptr;
    };

    glViewport(0, 0, viewportWidth, viewportHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glfwSwapInterval(0);

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    //glDebugMessageCallback(d_callback, NULL);

    return d_Window;
}
#endif