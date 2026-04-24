#include "./Keyboard.hpp"
#include <GLFW/glfw3.h>


ENGINE::Keyboard::Keyboard()
{

}

void ENGINE::Keyboard::processInput(GLFWwindow *window)
{

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}