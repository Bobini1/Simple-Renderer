#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shaders.h"
#include "misc/stb_image.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"
#include "../Renderer.h"
#include "Vertex.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

class OpenGLRenderer :public Renderer
{
private:
	const int EBOSize = 2'000'000 * sizeof(unsigned int), VBOSize = 50'000 * 10 * sizeof(Vertex);
	const float zBufferResolution = 0.000001f;

	struct Texture;

	GLFWwindow* window;
	Shader* simplestShader;

	unsigned int mainVAO;
	unsigned int mainVBO;
	unsigned int mainEBO;

	Vertex* vertices;
	unsigned int* indices;

	unsigned int arrayTexture;
	unsigned int dynamicEBOoffset, dynamicVBOoffset;

	unsigned int screenWidth, screenHeight;

	bool TexturesInitialized;
	bool regenMipmaps;

	std::unordered_map<std::string, Texture> textures;
	float currentSpotInArrayTexture;
	unsigned int maxTexWidth, maxTexHeight;
	unsigned int textureSlots;

	float drawOrder;
public:
	/**
	 * @brief 
	 * @param screenWidth 
	 * @param screenHeight 
	 * @param windowName 
	 * @param backgroundColor The rgba color of the background.
	*/
	OpenGLRenderer(unsigned int screenWidth = 800, unsigned int screenHeight = 600, std::string windowName = "", glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	virtual void TexInit(unsigned int maxTexWidth, unsigned int maxTexHeight, unsigned int numTextures) override;
	virtual void loadTexture(std::string texPath) override;

	virtual void drawRectangle(glm::vec2 origin, glm::vec2 size, glm::vec4 color) override;
	virtual void drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color) override;
	virtual void drawRectangle(glm::vec2 origin, glm::vec2 size, std::string texPath) override;
	virtual void drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, std::string texPath) override;

	virtual void draw(IDrawable* drawable) override;

	virtual void Display() override;

	~OpenGLRenderer();

private:
	/**
	 * @brief Utility function used to display OpenGLErrors.
	*/
	void checkGLError();

	/**
	 * @brief Called in @ref display(). Adds data to dynamic buffers.
	 * @param vertexPositions The vector of vertex positions of the shape.
	 * @param elements The triangles of a shape.
	 * @param texPath The texture of the shape.
	 * @param texCoords The texture coordinates of each vertex. <0.0, 1.0> range.
	 * @param color The color of the shape.
	*/
	void generate(std::vector<glm::vec2> vertexPositions, std::vector<unsigned int> elements, std::string texPath, std::vector<glm::vec2> texCoords, glm::vec4 color);

	/**
	 * @brief Adds a texture to the OpenGL texture storage.
	 * @param path The path of the texture to be loaded.
	 * @param width The pointer used to pass the width of the added texture.
	 * @param height The pointer used to pass the height of the added texture.
	 * @return The "slot" of the added texture in the texture array buffer.
	*/
	float addTex(std::string path, int* width, int* height);
};
