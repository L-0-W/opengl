
#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

namespace ENGINE {

    using Generic = std::variant<float, int, double, std::array<float, 4>, std::array<float, 3>>;

    class Shader {

        public:
            Shader();
            ~Shader() = default;

            void ourShader(std::string vertex_path, std::string fragment_path);
            void use();


            void addUniform(Generic &value, std::string name);
            unsigned int getRawShader();
        private:
            std::pair<std::string, std::string> loadShadersFile(std::string vertex_path, std::string fragment_path);
            std::unordered_map<std::string, Generic> uniforms;

            unsigned int shaderProgram;
    
            void setUniform(Generic value, std::string uniform);
    };
};