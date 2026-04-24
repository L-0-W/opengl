#include "./Mesh.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include <string>
#include <vector>
#include "../lib/stb_image.cpp"



ENGINE::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    setupMesh();
}

void ENGINE::Mesh::Draw(unsigned int shaderProgram)
{
    // for(const auto &texture: textures)
    // {
    //     std::println("{}", texture.type);

    //     std::println("{}", textures.size());

    //     if (textures.empty())
    //     {
    //       glActiveTexture(GL_TEXTURE0);    
    //     } 
    //     else 
    //     {
    //       glActiveTexture(GL_TEXTURE0 + (textures.size()-1)); 
    //     }
                
  
    //     glActiveTexture(GL_TEXTURE0 + (textures.size()-1));
    //     glBindTexture(GL_TEXTURE_2D, texture.id);
    // }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0].id);

    std::println("{}", textures[1].type);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1].id);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


    glBindVertexArray(0);
}

void ENGINE::Mesh::addTexture(std::string texture_path, std::string type, unsigned int shaderProgram)
{
    ENGINE::Texture texture;

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
}

ENGINE::Mesh ENGINE::Mesh::TRIANGLE()
{

    std::vector<ENGINE::Vertex> vertices;
    std::vector<unsigned int> indices;

vertices.push_back(Vertex{ 
    .Position = glm::vec3(-0.5f, 0.5f, 0.0f), 
    .TexCoords = glm::vec2(0.0f, 5.0f)   // ← Canto superior esquerdo (5 repetições)
});
vertices.push_back(Vertex{ 
    .Position = glm::vec3(0.5f, 0.5f, 0.0f), 
    .TexCoords = glm::vec2(5.0f, 5.0f)   // ← Canto superior direito
});
vertices.push_back(Vertex{ 
    .Position = glm::vec3(0.0f, 0.0f, 0.0f), 
    .TexCoords = glm::vec2(2.5f, 0.0f)   // ← Centro da base
});

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    

    auto mesh = ENGINE::Mesh(vertices, indices);

    return mesh;
}

void ENGINE::Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, TexCoords));

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}
