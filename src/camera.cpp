#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "convert.hpp"

glm::mat4 projection_matrix(const Camera& camera) {
    return glm::lookAt(camera.position, camera.direction, camera.up);
}

void move_camera(Camera& camera, const glm::vec3& direction) {
    camera.position += to_vec4(direction * 0.1f);
}
