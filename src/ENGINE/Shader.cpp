#include "./Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <fstream>
#include <glad/glad.h>
#include <ios>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

template<typename... TS>
struct Overloaded : TS... {
    using TS::operator()...;
};

template<typename... TS>
Overloaded(TS...) -> Overloaded<TS...>;

Shader::Shader() {}

std::pair<std::string, std::string> Shader::loadShadersFile(std::string vertex_path, std::string fragment_path)
{

    std::ifstream vertex_file(vertex_path, std::ios::binary | std::ios::ate);
    std::ifstream fragment_file(fragment_path, std::ios::binary | std::ios::ate);

    if (!vertex_file)
    {
        std::println("NÂO FOI POSSIVEL ABRIR VERTEX SHADER");
        throw std::runtime_error("NÂO FOI POSSIVEL ABRIR VERTEX SHADER");
    }

    if (!fragment_file)
    {
        std::println("NÂO FOI POSSIVEL ABRIR FRAGMENT SHADER");
        throw std::runtime_error("NÂO FOI POSSIVEL ABRIR FRAGMENT SHADER");    
    }


    std::streamsize vertex_size_file = vertex_file.tellg();
    std::streamsize fragment_size_file = fragment_file.tellg();

    vertex_file.seekg(0, std::ios::beg);
    fragment_file.seekg(0, std::ios::beg);

    std::string vertex_buffer(vertex_size_file, '\0');
    std::string fragment_buffer(fragment_size_file, '\0');

    if (!vertex_file.read(vertex_buffer.data(), vertex_size_file))
    {
        std::println("NÂO FOI POSSIVEL LER VERTEX SHADER!");
        throw std::runtime_error("NÂO FOI POSSIVEL LER VERTEX SHADER!");
    }

    if (!fragment_file.read(fragment_buffer.data(), fragment_size_file))
    {
        std::println("NÂO FOI POSSIVEL LER FRAGMENTO SHADER!");
        throw std::runtime_error("NÂO FOI POSSIVEL LER FRAGMENT SHADER!");
    }

    return {vertex_buffer, fragment_buffer};
}

void Shader::ourShader(std::string vertex_path, std::string fragment_path)
{
    unsigned int vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    auto shaders = this->loadShadersFile(vertex_path, fragment_path);

    const char *vertexSource = shaders.first.c_str(); 
    const char *fragmentSource = shaders.second.c_str();

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    this->shaderProgram = glCreateProgram();

    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);

    glLinkProgram(this->shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::changeFragment(std::string fragment_path)
{
    unsigned int fragmentShader;

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    auto shaders = this->loadShaderFile(fragment_path);

    const char *fragmentSource = shaders.c_str();


    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    glDeleteShader(fragmentShader);
}

std::string Shader::loadShaderFile(std::string fragment_path)
{
    std::ifstream fragment_file(fragment_path, std::ios::binary | std::ios::ate);

    if (!fragment_file)
    {
        std::println("NÂO FOI POSSIVEL ABRIR FRAGMENT SHADER");
        throw std::runtime_error("NÂO FOI POSSIVEL ABRIR FRAGMENT SHADER");    
    }


    std::streamsize fragment_size_file = fragment_file.tellg();

    fragment_file.seekg(0, std::ios::beg);

    std::string fragment_buffer(fragment_size_file, '\0');


    if (!fragment_file.read(fragment_buffer.data(), fragment_size_file))
    {
        std::println("NÂO FOI POSSIVEL LER FRAGMENTO SHADER!");
        throw std::runtime_error("NÂO FOI POSSIVEL LER FRAGMENT SHADER!");
    }

    return fragment_buffer;
}

void Shader::use()
{
    glUseProgram(this->shaderProgram);

    for(auto uniform: this->uniforms)
    {
        this->setUniform(uniform.second, uniform.first);
    }
}

void Shader::setUniform(Generic value, std::string uniform)
{
    int vertexLocation = glGetUniformLocation(this->shaderProgram, uniform.c_str());
   
    std::visit(Overloaded(
        [vertexLocation](int i) { glUniform1i(vertexLocation, i); },
        [vertexLocation](float i) { glUniform1f(vertexLocation, i); },
        [vertexLocation] (double i) { glUniform1d(vertexLocation, i); },
        [vertexLocation] (std::array<float, 4> i) { glUniform4f(vertexLocation, i[0], i[1], i[2], i[3]); },
        [vertexLocation] (std::array<float, 3> i) { glUniform3f(vertexLocation, i[0], i[1], i[2]); },
        [vertexLocation] (glm::mat4 mat4) { glUniformMatrix4fv(vertexLocation, 1, GL_FALSE, glm::value_ptr(mat4)); }
    ), value); 
}

void Shader::addUniform(Generic &value, std::string name)
{

    if (this->uniforms.find(name) != this->uniforms.end())
    {
       
        this->uniforms[name] = value;
        return;
    }

    std::println("NOME: {}", name);
    this->uniforms.insert({name, value});
}

unsigned int Shader::getRawShader()
{
    return this->shaderProgram;
}