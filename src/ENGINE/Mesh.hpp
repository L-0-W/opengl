
#include "glm/glm.hpp"
#include <string>
#include <vector>

namespace ENGINE {
    struct Vertex {
        glm::vec3 Position;
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

            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            static Mesh TRIANGLE();

            void addTexture(std::string texture_path, std::string type, unsigned int shaderProgram);
            void Draw(unsigned int shaderProgram)
            ;
            ~Mesh() = default;
        private:
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;

            void setupMesh();
    };
}