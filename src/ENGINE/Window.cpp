#include "./Window.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <print>

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Window::initGLFW()
{
    if(!glfwInit())
    {
        std::println("NÂO FOI POSSIVEL INICAR GLFW");
        return false;
    }    

    return true;
}

Window::Window()
{
    initGLFW();

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

void Window::swapBuffers()
{
    glfwSwapBuffers(this->rawWindow);
}

GLFWwindow* Window::getRawWindow()
{
   return this->rawWindow;
}
