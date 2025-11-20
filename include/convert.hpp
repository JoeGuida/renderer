#ifndef RENDERER_CONVERT_HPP
#define RENDERER_CONVERT_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

glm::vec4 to_vec4(const glm::vec3& v);
glm::vec4 to_vec4(float s);

#endif
