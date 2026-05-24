#pragma  once
#include "glm/ext/matrix_float4x4.hpp"
#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>


    using Generic = std::variant<float, int, double, std::array<float, 4>, std::array<float, 3>, glm::mat4>;

    class Shader {

        public:
            Shader();
            ~Shader() = default;

            void ourShader(std::string vertex_path, std::string fragment_path);
            void changeFragment(std::string fragment_path);
            
            void use();
            void setUniform(Generic value, std::string uniform);


            void addUniform(Generic &value, std::string name);
            unsigned int getRawShader();
        private:
            std::pair<std::string, std::string> loadShadersFile(std::string vertex_path, std::string fragment_path);
            std::string loadShaderFile(std::string fragment_path);

            std::unordered_map<std::string, Generic> uniforms;

            unsigned int shaderProgram;
    
    };
