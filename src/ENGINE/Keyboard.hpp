#include <GLFW/glfw3.h>



namespace ENGINE {
    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard() = default;

            void processInput(GLFWwindow *window);
        private:

    };
};