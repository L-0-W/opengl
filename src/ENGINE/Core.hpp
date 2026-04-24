#pragma  once
#include "./Keyboard.hpp"
#include "./Window.hpp"
#include "./Shader.hpp"
#include <GLFW/glfw3.h>
#include <optional>


namespace ENGINE {


    class Core 
    {
        public:
            //INITS ~ glfwInit(), gladInit, etc...
            static std::optional<Core> create();
            static void destroy();
            void draw();
            
            void clear();
            void swap();


            ENGINE::Window window;
            ENGINE::Shader shader;
            ENGINE::Keyboard keyboard;
            ~Core() = default;

            void configureVertex();
        private:
            Core();
            unsigned int VBO;
            unsigned int VAO;
            unsigned int EBO;
    };
};