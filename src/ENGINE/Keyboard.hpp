#include <GLFW/glfw3.h>




    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard() = default;

            int processInput(GLFWwindow *window);
        private:

    };
