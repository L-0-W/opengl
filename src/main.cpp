#include "ENGINE/Core.hpp"
#include "ENGINE/Mesh.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>

int main()
{

    auto core  = ENGINE::Core::create();
    
    auto tri = ENGINE::Mesh::TRIANGLE();


    double ypos, xpos;
    int width, height;

    if (core.has_value())
    {

        core->shader.ourShader("../res/vertex.vs", "../res/fragment.frag");
        core->shader.use();

        tri.addTexture("../res/wall.jpg", "Wall", core->shader.getRawShader());
        tri.addTexture("../res/lig.png", "Ligth", core->shader.getRawShader());

        while(!glfwWindowShouldClose(core->window.getRawWindow()))
        {
            core->clear();
            
            core->keyboard.processInput(core->window.getRawWindow());
            core->shader.use();

            glfwGetCursorPos(core->window.getRawWindow(), &xpos, &ypos);
            glfwGetWindowSize(core->window.getRawWindow(), &width, &height);

            tri.Draw(core->shader.getRawShader());

            float timeValue = glfwGetTime();
            float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

            ENGINE::Generic value = std::array<float, 4>{1.0, greenValue, 1.0, 1.0};
            core->shader.addUniform(value, "ourColor");

            ENGINE::Generic uniformXPos = (float)xpos;
            ENGINE::Generic uniformYPos = (float)height - (float)ypos;

            //std::println("X: {} - y: {}",  (float)xpos, (float)ypos);


            core->shader.addUniform(uniformXPos, "xPos");
            core->shader.addUniform(uniformYPos, "yPos");
            
            core->swap();
        }

        core->destroy();
    }

    return 0;
}

