#include "./Window.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <print>

void ENGINE::Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

ENGINE::Window::Window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->rawWindow = glfwCreateWindow(this->WIDTH, this->HEIGHT, "opengl", NULL, NULL);

    if (this->rawWindow == NULL)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
    }
    glfwMakeContextCurrent(this->rawWindow);
    glfwSetFramebufferSizeCallback(this->rawWindow, this->framebuffer_size_callback);
};

void ENGINE::Window::swapBuffers()
{
    glfwSwapBuffers(this->rawWindow);
}

GLFWwindow* ENGINE::Window::getRawWindow()
{
   return this->rawWindow;
}
