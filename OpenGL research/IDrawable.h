#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class IDrawable
{
public:
	/**
	 * @brief Used to retrieve the center of the drawable, for example for scaling.
	 * It is calculated by averaging all points forming the shape.
	 * @return The center of a shape.
	*/
	virtual glm::vec2 getCenterPoint() = 0;

	/**
	 * @brief Used to create a copy of the drawable.
	 * @return The copy.
	*/
	virtual IDrawable* clone() = 0;

	/**
	 * @brief Used to retrieve the texture path of the drawable.
	 * @return The texture path assigned to the drawable.
	*/
	virtual std::string getTexPath() = 0;

	/**
	 * @brief Used to retrieve the triangle formation data.
	 * @return a vector of vector index combinations that form the triangles of the drawable.
	*/
	virtual std::vector<unsigned int> getElementData() = 0;

	/**
	 * @brief Used to retrieve the vertex position data.
	 * @return A vector of all vertex positions.
	*/
	virtual std::vector<glm::vec2> getVertexData() = 0;

	/**
	 * @brief Used to retrieve the texture coordinate data.
	 * @return A vector of texture coordinates for each vertex.
	*/
	virtual std::vector<glm::vec2> getTexCoordsData() = 0;

	/**
	 * @brief Used to retrieve the current color of the drawable.
	 * @return An rgba shape 4-dimensional vector.
	*/
	virtual glm::vec4 getColor() = 0;

	/**
	 * @brief Sets the color of the drawable.
	 * @param color The color in the rgba format.
	*/
	virtual void setColor(glm::vec4 color) = 0;

	/**
	 * @brief Retrieves the current translation of the drawable.
	 * @return The translation vector. vec2(0.0f, 0.0f) is the default.
	*/
	virtual glm::vec2 getTranslation() = 0;

	/**
	 * @brief Sets the translation of the drawable.
	 * @param translation The translation vector. vec2(0.0f, 0.0f) is the default.
	*/
	virtual void setTranslation(glm::vec2 translation) = 0;

	/**
	 * @brief Used to retrieve the current scaling applied to the drawable.
	 * @return The scale vector. vec2(1.0f, 1.0f) is the default.
	*/
	virtual glm::vec2 getScale() = 0;

	/**
	 * @brief Sets the scaling of the drawable.
	 * @param scale The scale vector. vec2(1.0f, 1.0f) is the default.
	*/
	virtual void setScale(glm::vec2 scale) = 0;

	// unimplemented
	/*virtual glm::mat4 getRotation() = 0;
	virtual void setRotation(glm::mat4 rotation) = 0;*/
};
