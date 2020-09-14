#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Shader.h"

glm::mat4 ModelMatrix = glm::mat4(1.0f); //Identity matrix :: Model coordinates => World coordinates
glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f)); // World coordinates => Camera coordinates