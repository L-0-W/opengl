#include "./Core.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <print>


float vertices[] = {
    -0.5f, 0.5f, 0.0f, // A - 0
    0.5f, 0.5f, 0.0f, // B - 1
    0.0f, 0.0f, 0.0f, // MEIO - 2
};

unsigned int indices [] = {
    0, 1, 2,
};

std::optional<ENGINE::Core> ENGINE::Core::create()
{
    glfwInit();
    auto core = ENGINE::Core();
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::println("Failed to initialize GLAD");
        return std::nullopt;
    }    

    core.configureVertex();

    return core;
};

void ENGINE::Core::destroy()
{
    glfwTerminate();
}

void ENGINE::Core::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ENGINE::Core::swap()
{
    this->window.swapBuffers();
    glfwPollEvents();
}

void ENGINE::Core::draw()
{

    glBindVertexArray(this->VAO);           
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}


void ENGINE::Core::configureVertex()
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
}


ENGINE::Core::Core()
{
    this->window = ENGINE::Window();
    this->keyboard = ENGINE::Keyboard();
    this->shader = ENGINE::Shader();

    std::println("CORE CREATED!");
}