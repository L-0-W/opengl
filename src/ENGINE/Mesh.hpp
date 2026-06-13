
#pragma  once
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <array>
#include <string>
#include <vector>


    enum AXIS {
        X,
        Y,
        Z,
    };

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normals;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
    };

    class Mesh {
        public:
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;
            glm::mat4 transform;
            glm::vec3 pos;

            std::array<glm::vec3, 3> lightsPosition;
            glm::vec3 Sol;

            std::array<glm::vec3, 6> cubePositions;

            unsigned int VAO;

            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec3 pos);
            static Mesh TRIANGLE(glm::vec3 pos);

            void addTexture(std::string texture_path, std::string type, unsigned int shaderProgram);

            void tranlate();
            void scale(glm::vec3 scaleVec);
            void rotate(float angle, AXIS axis);

            void Draw();

            ~Mesh() = default;
        private:
            unsigned int VBO;
            unsigned int EBO;

            glm::vec3 axis = glm::vec3(0.0f);

            void setupMesh();
    };
