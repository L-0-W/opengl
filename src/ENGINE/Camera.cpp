#include "./Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

void Camera::update()
{
    this->cameraTarget = cameraPos + cameraFront;

    glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f);

    this->cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
    this->cameraRigth = glm::normalize(glm::cross(wordlUp, this->cameraDirection));
    this->cameraUp = glm::cross(this->cameraDirection, this->cameraRigth);

    glm::mat4 ori(
        glm::vec4(this->cameraRigth, 0.0f),
        glm::vec4(this->cameraUp, 0.0f),
        glm::vec4(this->cameraDirection, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    glm::mat4 rotacao = glm::transpose(ori);
    glm::mat4 transformacao = glm::translate(glm::mat4(1.0f), -this->cameraPos);

    this->view = rotacao * transformacao;
}

Camera::Camera()
{
    this->cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);

    this->cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    this->cameraDirection = glm::normalize(this->cameraPos - this->cameraTarget);
    
    glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    this->cameraRigth = glm::normalize(glm::cross(wordlUp, this->cameraDirection));
    this->cameraUp = glm::cross(this->cameraDirection, this->cameraRigth);

    glm::mat4 ori(
        glm::vec4(this->cameraRigth, 0.0f),
        glm::vec4(this->cameraUp, 0.0f),
        glm::vec4(this->cameraDirection, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    glm::mat4 rotacao = glm::transpose(ori);
    glm::mat4 transformacao = glm::translate(glm::mat4(1.0f), -this->cameraPos);

    this->view = rotacao * transformacao;
}
