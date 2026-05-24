#include "./Core.hpp"
#include "./Window.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"

#include <glad/glad.h>
#include <print>
#include <GLFW/glfw3.h>


float vertices[] = {
    -0.5f, 0.5f, 0.0f, // A - 0
    0.5f, 0.5f, 0.0f, // B - 1
    0.0f, 0.0f, 0.0f, // MEIO - 2
};

unsigned int indices [] = {
    0, 1, 2,
};

Core& Core::getInstace()
{
    static Core instance;
    return instance;
};


bool Core::initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::println("Failed to initialize GLAD");
        return true;
    }    

    glEnable(GL_DEPTH_TEST);
    return false;
}

void Core::destroy()
{
    glfwTerminate();
}

void Core::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Core::swap()
{
    this->window.swapBuffers();
    glfwPollEvents();
}

void Core::draw()
{

    glBindVertexArray(this->VAO);           
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}


void Core::configureVertex()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // SECOND VAO - LIGTH VAO

    glGenVertexArrays(1, &this->ligthVAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->ligthVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void Core::processInput()
{
    float currentFrame = glfwGetTime();

    this->deltatime = currentFrame - lastframe;
    this->lastframe = currentFrame; 
    
    float cameraSpeed = 5.0f * this->deltatime;

    auto targetZ = cameraSpeed * this->camera.cameraFront;

    if(glfwGetKey(this->window.getRawWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window.getRawWindow(), true);

    if(glfwGetKey(this->window.getRawWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        this->camera.cameraPos += glm::vec3(targetZ.x, 0.0f, targetZ.z);    
    }

    if(glfwGetKey(this->window.getRawWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        this->camera.cameraPos -= glm::vec3(targetZ.x, 0.0f, targetZ.z);
    }
    
    if(glfwGetKey(this->window.getRawWindow(), GLFW_KEY_D) == GLFW_PRESS)
        this->camera.cameraPos += glm::normalize(glm::cross(this->camera.cameraFront, this->camera.cameraUp)) * cameraSpeed;
    
    if(glfwGetKey(this->window.getRawWindow(), GLFW_KEY_A) == GLFW_PRESS)
        this->camera.cameraPos -= glm::normalize(glm::cross(this->camera.cameraFront, this->camera.cameraUp)) * cameraSpeed;


    // MOUSE INPUTS
}

Core::Core()
{
    initGLAD();

    configureVertex();

    std::println("CORE CREATED!");
}