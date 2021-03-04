#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @brief Defines the full data structure of a vertex.
*/
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	float layer;
};

