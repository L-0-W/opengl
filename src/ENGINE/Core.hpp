#pragma  once
#include "./Keyboard.hpp"
#include "./Window.hpp"
#include "./Shader.hpp"
#include "./Camera.hpp"
#include "./Mouse.hpp"
#include <GLFW/glfw3.h>

    class Core
    {
        public:
            static Core& getInstace();

            Core(const Core&) = delete;
            ~Core() = default;
            Core& operator=(const Core&) = delete;

            void draw();
            void clear();
            void swap();

            void processInput();

            float deltatime, lastframe = 0.0f;

            Window window;
            Shader shader;
            Shader lightShader;
            Keyboard keyboard;
            Mouse mouse;
            Camera camera;

            void configureVertex();
        private:
            Core();

            static bool initGLAD();

            static void destroy();

            unsigned int VBO;
            unsigned int VAO;
            unsigned int ligthVAO;
            unsigned int EBO;
    };
