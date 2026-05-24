#include "./Keyboard.hpp"
#include <GLFW/glfw3.h>
#include <print>


Keyboard::Keyboard()
{

}

int Keyboard::processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::println("CLOSE WINDOW");
        glfwSetWindowShouldClose(window, true);
        return 0;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        std::println("WALK FORWARD");
        return GLFW_KEY_W;
    }
    
    return 0;
}   