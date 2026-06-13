#include "./Mesh.hpp"
#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include <string>
#include <vector>
#include "../lib/stb_image.cpp"
#include "ENGINE/Core.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <glm/gtc/type_ptr.hpp>




Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec3 pos)
{
    this->vertices = vertices;
    this->indices = indices;
    this->transform = glm::mat4(1.0f);
    this->pos = pos;

    this->cubePositions =
    {
        glm::vec3(-0.5f, 0.0f, 1.0f),
        glm::vec3(-0.5f, 0.0f, -1.0f),
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec3( 1.0f, 1.0f, -1.0f),
        glm::vec3( 0.5f, -0.5f, 0.5f),
        glm::vec3(-1.0f, 0.0f, 1.0f),
    };

    this->lightsPosition =
    {
       glm::vec3(-0.5f, 2.0f, 1.0f),
       glm::vec3(-0.5f, -2.0f, 1.0f),
       glm::vec3(-2.0f, 0.0f, 1.0f)
    };

    setupMesh();
}
void Mesh::scale(glm::vec3 scaleVec)
{
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, scaleVec);

    this->transform =  trans * this->transform;
}

void Mesh::rotate(float angle, AXIS axis)
{
    glm::mat4 trans = glm::mat4(1.0f);

    switch (axis) {
        case AXIS::X:
            this->axis.x = 1.0f;
            break;
        case AXIS::Y:
            this->axis.y = 1.0f;
            break;
        case AXIS::Z:
            this->axis.z = 1.0f;
            break;
        default:
            this->axis.z = 1.0f;
    };

    std::println("X: {} - Y: {} - Z: {}", this->axis.x, this->axis.y, this->axis.z);

    glm::vec3 axisother = -this->axis;
    std::println("DOT: {}", glm::dot(this->axis, axisother));

    trans = glm::rotate(trans, angle, this->axis);
    this->transform = trans * this->transform;
};

void Mesh::Draw()
{


    auto& core = Core::getInstace();

    // --- 1. Bind State ONCE ---
    core.shader.use(); // Ensure shader is active

    // Bind textures once since they are shared by all instances in this loop

    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }


    glBindVertexArray(VAO);


    //SOL
    glm::mat4 modelSol = glm::mat4(1.0f);
    modelSol = glm::translate(modelSol, this->Sol);
    modelSol = glm::scale(modelSol, glm::vec3(3.5f, 3.5f, 3.5f));

    Generic modelSolValue = modelSol;

    core.lightShader.use();
    core.lightShader.setUniform(modelSolValue, "model");
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);


    int j = 0;
    while(j < lightsPosition.size())
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightsPosition[j]);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

        Generic modelValue = model;

        core.lightShader.use();
        core.lightShader.setUniform(modelValue, "model");

        if (j == 1)
        {
            Generic cubeColor = std::array<float, 3>{1.0f, 0.0f, 0.0f};
            core.lightShader.addUniform(cubeColor, "lightCubeColor");
        }

        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        j++;
    }
    // --- 2. The Loop (Only update what changes) ---
    int i = 0;
    while(i < cubePositions.size())
    {

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
//        model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 1.0f));

        // Use your wrapper to set the uniform
        Generic modelValue = model;
        //shader.addUniform(modelValue, "model");

        core.shader.use();
        core.shader.setUniform(modelValue, "model");

        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        i++;
    }

    // --- 3. Cleanup ---
    glBindVertexArray(0);
}

void Mesh::addTexture(std::string texture_path, std::string type, unsigned int shaderProgram)
{
    Texture texture;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);


    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;

    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::println("ERRO AO TENTAR CARREGAR IMAGEM");
        return;
    }

    GLenum internalFormat;
    GLenum dataFormat;

    if (nrChannels == 1) {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }
    else if (nrChannels == 3) {
        internalFormat = GL_RGB;   // JPG, sem alpha
        dataFormat = GL_RGB;
    }
    else if (nrChannels == 4) {
        internalFormat = GL_RGBA;  // PNG com alpha! ← SUA TEXTURA 2!
        dataFormat = GL_RGBA;
    }


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        dataFormat,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    int textureWrapping = type.contains("Ligth") ? GL_CLAMP_TO_BORDER : GL_REPEAT;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture.type = type;

    this->textures.push_back(texture);

    std::println("{}", textures.size()-1);
    glUniform1i(glGetUniformLocation(shaderProgram, std::format("texture{}", textures.size()-1).c_str()), textures.size()-1);

    stbi_image_free(data);
}

Mesh Mesh::TRIANGLE(glm::vec3 TRIpos)
{
    std::vector<unsigned int> indices;

    // 1. Posições dos 8 cantos do cubo
    glm::vec3 pos[] = {
        glm::vec3(-0.5f, -0.5f,  0.5f), // 0: Front-Bottom-Left
        glm::vec3( 0.5f, -0.5f,  0.5f), // 1: Front-Bottom-Right
        glm::vec3( 0.5f,  0.5f,  0.5f), // 2: Front-Top-Right
        glm::vec3(-0.5f,  0.5f,  0.5f), // 3: Front-Top-Left
        glm::vec3(-0.5f, -0.5f, -0.5f), // 4: Back-Bottom-Left
        glm::vec3( 0.5f, -0.5f, -0.5f), // 5: Back-Bottom-Right
        glm::vec3( 0.5f,  0.5f, -0.5f), // 6: Back-Top-Right
        glm::vec3(-0.5f,  0.5f, -0.5f)  // 7: Back-Top-Left
    };

    // 2. Normais de cada face
    glm::vec3 normalBack(0.0f, 0.0f, -1.0f);
    glm::vec3 normalFront(0.0f, 0.0f, 1.0f);
    glm::vec3 normalLeft(-1.0f, 0.0f, 0.0f);
    glm::vec3 normalRight(1.0f, 0.0f, 0.0f);
    glm::vec3 normalBottom(0.0f, -1.0f, 0.0f);
    glm::vec3 normalTop(0.0f, 1.0f, 0.0f);

    // 3. Coordenadas de textura padrão para o mapeamento do Quad
    glm::vec2 uv0(0.0f, 0.0f);
    glm::vec2 uv1(1.0f, 0.0f);
    glm::vec2 uv2(1.0f, 1.0f);
    glm::vec2 uv3(0.0f, 1.0f);

    // 4. Montagem estruturada dos Vértices (Posição, Normal, UV)
    std::vector<Vertex> vertices = {
        // Back Face (Baseado nas primeiras 6 linhas do array original)
        {pos[4], normalBack, uv0}, {pos[5], normalBack, uv1}, {pos[6], normalBack, uv2}, {pos[7], normalBack, uv3},

        // Front Face
        {pos[0], normalFront, uv0}, {pos[1], normalFront, uv1}, {pos[2], normalFront, uv2}, {pos[3], normalFront, uv3},

        // Left Face
        {pos[3], normalLeft, uv1}, {pos[7], normalLeft, uv2}, {pos[4], normalLeft, uv3}, {pos[0], normalLeft, uv0},

        // Right Face
        {pos[2], normalRight, uv1}, {pos[6], normalRight, uv2}, {pos[5], normalRight, uv3}, {pos[1], normalRight, uv0},

        // Bottom Face
        {pos[4], normalBottom, uv3}, {pos[5], normalBottom, uv2}, {pos[1], normalBottom, uv1}, {pos[0], normalBottom, uv0},

        // Top Face
        {pos[7], normalTop, uv3}, {pos[6], normalTop, uv2}, {pos[2], normalTop, uv1}, {pos[3], normalTop, uv0}
    };

    // 5. Geração automática dos Índices por Face (Mantido idêntico)
    for (int i = 0; i < 6; i++) {
        int offset = i * 4;
        // Triangle 1
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        // Triangle 2
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);
    }

    auto mesh = Mesh(vertices, indices, TRIpos);
    return mesh;
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, Normals));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, TexCoords));


    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}
