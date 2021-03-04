#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "misc/stb_image.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "IDrawable.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>


class Renderer
{
public:

	/**
	* @brief Initializes the texture functionality by creating immutable storage for textures.
	* @details This storage provides incredible speed at the cost of wasting vram if the textures aren't of similar sizes.
	This is because the width and height of the storage need to be able to fit the biggest texture.
	Smaller textures will use "slots" of the same size as the biggest texture.
	* @param maxTexWidth The height of the texture storage. Texture used in the program can't be higher than this.
	* @param maxTexHeight The width of the texture storage. Texture used in the program can't be wider than this.
	* @param numTextures The depth of the texture storage. There can't be more individual textures than this number in the program.
	*/
	virtual void TexInit(unsigned int maxTexWidth, unsigned int maxTexHeight, unsigned int numTextures) = 0;

	/**
	 * @brief Creates a rectangle on the stack that is immediately discarded after drawing.
	 * @note Can be used as a shorter alternative to @ref draw() without the need to create an object.
	 * @param origin The origin point (lower left corner). <-1.0, 1.0> range of coordinates.
	 * @param size The width and height of the rectangle.
	 * @param color The color of the rectangle in rgba format.
	*/
	virtual void drawRectangle(glm::vec2 origin, glm::vec2 size, glm::vec4 color) = 0;

	/**
	 * @brief Creates a quadrangle out of any four points on the stack that is immediately discarded after drawing.
	 * @note The order needs to be counter clockwise.\n
	 * Can be used as a shorter alternative to @ref draw() without the need to create an object.
	 * @param p1 Lower left corner. <-1.0, 1.0> range of coordinates.
	 * @param p2 Lower right corner. <-1.0, 1.0> range of coordinates.
	 * @param p3 Upper rigt corner. <-1.0, 1.0> range of coordinates.
	 * @param p4 Upper left corner. <-1.0, 1.0> range of coordinates.
	 * @param color The color of the rectangle in rgba format.
	*/
	virtual void drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color) = 0;

	/**
	 * @brief Creates a rectangle on the stack that is immediately discarded after drawing.
	 * @note Can be used as a shorter alternative to @ref draw() without the need to create an object.
	 * @param origin The origin point (lower left corner). <-1.0, 1.0> range of coordinates.
	 * @param size The width and height of the rectangle.
	 * @param texPath The path to the texture. A black shape will be drawn if it doesn't exist.
	*/
	virtual void drawRectangle(glm::vec2 origin, glm::vec2 size, std::string texPath) = 0;

	/**
	 * @brief Creates a quadrangle out of any four points on the stack that is immediately discarded after drawing.
	 * @note The order needs to be counter clockwise. It should start from the lower left corner if you want the texture to be positioned upright.\n
	 * Can be used as a shorter alternative to @ref draw() without the need to create an object.
	 * @param p1 Lower left corner. <-1.0, 1.0> range of coordinates.
	 * @param p2 Lower right corner. <-1.0, 1.0> range of coordinates.
	 * @param p3 Upper rigt corner. <-1.0, 1.0> range of coordinates.
	 * @param p4 Upper left corner. <-1.0, 1.0> range of coordinates.
	 * @param texPath The path to the texture. A black shape will be drawn if it doesn't exist.
	*/
	virtual void drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, std::string texPath) = 0;

	/**
	 * @brief Displays everything that was drawn before calling it.
	 * @note You need to re-draw shapes and display them for each frame you want to generate.
	*/
	virtual void Display() = 0;

	/**
	 * @brief Allows you to load a texture before using it.
	 * @note If this is not used, the texture will be loaded on its first use in the program, which can cause unwanted delays.
	 * @param texPath The path to the texture.
	*/
	virtual void loadTexture(std::string texPath) = 0;

	/**
	 * @brief Draws an IDrawable object on the screen.
	 * @param drawable A drawable object pointer.
	*/
	virtual void draw(IDrawable* drawable) = 0;

	virtual ~Renderer() {};
};
