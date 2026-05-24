#pragma once

#include <GLFW/glfw3.h>

    class Mouse {
        public:
            Mouse();
            ~Mouse() = default;

            static bool firstMouse;
            static double lastX, lastY;
            static float yaw, pitch;
            
            static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
            
        private:
    };

