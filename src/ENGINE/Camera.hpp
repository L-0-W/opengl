#pragma  once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

    class Camera
    {
        public:
            glm::vec3 cameraPos;
            glm::vec3 cameraTarget;
            glm::vec3 cameraDirection;
            glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

            glm::vec3 cameraUp;
            glm::vec3 cameraRigth;
            
            glm::mat4 view;

            void update();
            
            Camera();
            ~Camera() = default;
        private:
    };

