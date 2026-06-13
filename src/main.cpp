#include "./ENGINE/ENGINE.hpp"
#include "ENGINE/Mesh.hpp"
#include "ENGINE/Shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <array>
#include <cmath>
#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>


int main()
{

    auto& core  = Core::getInstace();
    auto tri = Mesh::TRIANGLE(glm::vec3(0.0f, 0.0f, -10.0f));

    double ypos, xpos;
    int width, height;


    core.shader.ourShader("../res/vertex.vs", "../res/fragment.frag");
    core.shader.use();

    core.lightShader.ourShader("../res/vertex.vs", "../res/fragmentLight.frag");

    tri.addTexture("../res/diffuseMap.png", "DiffuseMap", core.shader.getRawShader());
    tri.addTexture("../res/specularMap.png", "DiffuseMap", core.shader.getRawShader());

    tri.addTexture("../res/matrixEmission.jpg", "DiffuseMap", core.shader.getRawShader());

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

        tri.Sol = glm::vec3(0.0f, 100.0f, 0.0f);

        tri.Draw();

        core.camera.update();
        //core.camera.update();

        // glm::mat4 view;
        // view = glm::lookAt(glm::vec3(camx, 0.0f, camz), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);


        Generic viewValue = core.camera.view;
        Generic projectionValue = projection;


        Generic colorValue = std::array<float, 3>{1.0, 0.5, 1.0};
        Generic lightColorValue = std::array<float, 3>{1.0, 1.0, 1.0};
        Generic cameraPosValue = std::array<float, 3>{core.camera.cameraPos.x, core.camera.cameraPos.y, core.camera.cameraPos.z};

        Generic ambientMaterialColor = std::array<float, 3>{0.0215f, 0.1745f, 0.0215f};
        Generic diffuseMapIndex = int{0};
        Generic specularMapIndex = int{1};
        Generic shininessMaterialColor = float{32.0f};

        glm::vec3 lightColor;
        glm::vec3 ambientLight;

        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        lightColor = lightColor * glm::vec3(0.5f);
        ambientLight = lightColor * glm::vec3(0.2f);

        Generic diffuseLightColor = std::array<float, 3>{0.5f, 0.5f, 0.5f};
        Generic ambientLightColor = std::array<float, 3>{0.1f, 0.1f, 0.1f};
        Generic specularLightColor = std::array<float, 3>{1.0f, 1.f, 1.0f};



        core.shader.addUniform(viewValue, "view");
        core.shader.addUniform(projectionValue, "projection");
        core.shader.addUniform(cameraPosValue, "viewPos");


        Generic emissionIdex = int{2};

        core.shader.addUniform(ambientMaterialColor, "material.ambient");
        core.shader.addUniform(diffuseMapIndex, "material.diffuse");
        core.shader.addUniform(emissionIdex, "material.emission");

        core.shader.addUniform(specularMapIndex, "material.specular");
        core.shader.addUniform(shininessMaterialColor, "material.shininess");

        glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, 0.0f) - tri.Sol;
        Generic lightSourceValue = std::array<float, 3>{lightDirection.x, lightDirection.y, lightDirection.z};

        core.shader.addUniform(lightSourceValue, "dirLight.direction");
        core.shader.addUniform(ambientLightColor, "dirLight.ambient");
        core.shader.addUniform(diffuseLightColor, "dirLight.diffuse");
        core.shader.addUniform(specularLightColor, "dirLight.specular");

        Generic flashlightCutoffValue{glm::cos(glm::radians(10.5f))};
        Generic flashlightOuterCutoffValue{glm::cos(glm::radians(12.5f))};
        Generic flashlightPositionValue = std::array<float,3>{core.camera.cameraPos.x, core.camera.cameraPos.y, core.camera.cameraPos.z};
        Generic flashlightDirectionValue = std::array<float,3>{core.camera.cameraFront.x, core.camera.cameraFront.y, core.camera.cameraFront.z};

        core.shader.addUniform(flashlightPositionValue, "spotLight.position");
        core.shader.addUniform(flashlightDirectionValue, "spotLight.direction");
        core.shader.addUniform(flashlightCutoffValue, "spotLight.cutoff");
        core.shader.addUniform(flashlightOuterCutoffValue, "spotLight.outerCutOff");

        Generic constant{1.0f};
        Generic linear{0.022f};
        Generic quadratic{0.0019f};


        int i = 0;
        while(i < tri.lightsPosition.size())
        {

            auto lightStringIndex = std::format("pointLights[{}]", i);

            Generic lightSourceValuePoint = std::array<float, 3>{tri.lightsPosition[i].x, tri.lightsPosition[i].y, tri.lightsPosition[i].z};

            core.shader.addUniform(lightSourceValuePoint, std::format("{}.position", lightStringIndex));

            std::println("{}.position - {}", lightStringIndex, tri.lightsPosition[i].x);

            core.shader.addUniform(constant, std::format("{}.constant", lightStringIndex));
            core.shader.addUniform(linear,std::format("{}.linear", lightStringIndex));
            core.shader.addUniform(quadratic, std::format("{}.quadratic", lightStringIndex));


            core.shader.addUniform(ambientLightColor, std::format("{}.ambient", lightStringIndex));
            core.shader.addUniform(diffuseLightColor, std::format("{}.diffuse", lightStringIndex));
            core.shader.addUniform(specularLightColor, std::format("{}.specular", lightStringIndex));

            i++;
        }


        Generic lightPointColor1 = std::array<float, 3>{1.0f, 0.0f, 0.0f};
        core.shader.addUniform(lightPointColor1, "pointLights[0].lightColor");

        Generic lightPointColor2 = std::array<float, 3>{1.0f, 0.0f, 0.0f};
        core.shader.addUniform(lightPointColor2, "pointLights[1].lightColor");


        Generic lightPointColor3 = std::array<float, 3>{1.0f, 0.0f, 0.0f};
        core.shader.addUniform(lightPointColor3, "pointLights[2].lightColor");


        core.lightShader.use();
        core.lightShader.addUniform(viewValue, "view");
        core.lightShader.addUniform(projectionValue, "projection");
        core.lightShader.addUniform(colorValue, "objectColor");
        core.lightShader.addUniform(lightColorValue, "lightColor");
        core.lightShader.addUniform(ambientLightColor, "lightCubeColor");

        core.swap();
    }


    return 0;
}
