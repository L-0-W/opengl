#pragma once

#include <GLFW/glfw3.h>


    class Window 
    {
        public:
            Window();
            ~Window() = default;

            void swapBuffers();
            GLFWwindow* getRawWindow();
        private:
            GLFWwindow* rawWindow;
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

            static bool initGLFW();

            int WIDTH = 800;
            int HEIGHT = 600;
    };
