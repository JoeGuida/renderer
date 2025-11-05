#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 projection_matrix(const Camera& camera) {
    return glm::lookAt(camera.position, camera.direction, camera.up);
}

