#include "./ENGINE/ENGINE.hpp"
#include "ENGINE/Mesh.hpp"
#include "ENGINE/Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <array>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main()
{

    auto& core  = Core::getInstace();
    auto tri = Mesh::TRIANGLE(glm::vec3(0.0f, 0.0f, -10.0f));

    double ypos, xpos;
    int width, height;


    core.shader.ourShader("../res/vertex.vs", "../res/fragment.frag");
    core.shader.use();

    core.lightShader.ourShader("../res/vertex.vs", "../res/fragmentLight.frag");

    glfwSetInputMode(core.window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(core.window.getRawWindow(), core.mouse.mouse_callback);

    while(!glfwWindowShouldClose(core.window.getRawWindow()))
    {
        core.clear();

        glm::vec3 pos(0.0, 0.0f, -5.0);

        core.processInput();
        core.shader.use();

        glfwGetCursorPos(core.window.getRawWindow(), &xpos, &ypos);
        glfwGetWindowSize(core.window.getRawWindow(), &width, &height);

        glm::mat4 trans = glm::mat4(1.0f); // - Matrix Identidade
        float timeValue = glfwGetTime();

        tri.lightPosValue = glm::vec3(cos(timeValue) * 2.0f, sin(timeValue) * 2.0f, -1.0f);

        tri.Draw();


        float camx = sin(timeValue) * 10.0f;
        float camz = cos(timeValue) * 10.0f;

        core.camera.update();
        //core.camera.update();

        // glm::mat4 view;
        // view = glm::lookAt(glm::vec3(camx, 0.0f, camz), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);


        Generic viewValue = core.camera.view;
        Generic projectionValue = projection;


        Generic colorValue = std::array<float, 3>{1.0, 0.5, 1.0};
        Generic lightColorValue = std::array<float, 3>{1.0, 1.0, 1.0};
        Generic lightPosValue = std::array<float, 3>{tri.lightPosValue.x, tri.lightPosValue.y, tri.lightPosValue.z};

        core.shader.addUniform(viewValue, "view");
        core.shader.addUniform(projectionValue, "projection");
        core.shader.addUniform(colorValue, "objectColor");
        core.shader.addUniform(lightColorValue, "lightColor");
        core.shader.addUniform(lightPosValue, "lightPos");

        core.lightShader.use();
        core.lightShader.addUniform(viewValue, "view");
        core.lightShader.addUniform(projectionValue, "projection");
        core.lightShader.addUniform(colorValue, "objectColor");
        core.lightShader.addUniform(lightColorValue, "lightColor");

        core.swap();
    }


    return 0;
}
