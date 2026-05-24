
#include "./Mouse.hpp"
#include "ENGINE/Core.hpp"
#include "glm/trigonometric.hpp"
#include <print>


bool Mouse::firstMouse = true;

double Mouse::lastX = 0; 
double Mouse::lastY = 0; 

float Mouse::yaw = 0;
float Mouse::pitch = 0;


Mouse::Mouse(){
    std::println("MOUSE CRIADO");
}

void Mouse::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    
    if (pitch < -89.0f)
        pitch = -89.0f;


    glm::vec3 direction;
    
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    auto& core = Core::getInstace();
    core.camera.cameraFront = glm::normalize(direction);
}