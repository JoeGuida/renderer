#ifndef GL_RENDERER_CAMERA_HPP
#define GL_RENDERER_CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#ifdef near
#undef near
#endif

#ifdef far
#undef far 
#endif

struct Camera {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;
    float fov_radians;
    float aspect;
    float near;
    float far;
};

glm::mat4 projection_matrix(const Camera& camera);
void move_camera(Camera& camera, const glm::vec3& direction);

#endif
