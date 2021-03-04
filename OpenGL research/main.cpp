#include <iostream>
#include <math.h>
#include "Renderer.h"
#include <chrono>
#include "OpenGLRenderer/OpenGLRenderer.h"
#include "IDrawable.h"
#include "OpenGLRenderer/Mesh.h"
#include "OpenGLRenderer/Rect.h"

int main()
{
	Renderer* renderer = new OpenGLRenderer(1600, 900, "renderer", glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	renderer->TexInit(3000, 7000, 82);
	IDrawable* meshes[80];
	IDrawable* rects[80];

	std::vector <glm::vec2> vertices = {
		glm::vec2(0.5f, 0.5f), // top right
		glm::vec2(0.5f, -0.5f), // bottom right
		glm::vec2(-0.5f, -0.5f), // bottom left
		glm::vec2(-0.5f, 0.5f) // top left
	};
	std::vector<unsigned int> indices = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};
	std::vector<glm::vec2> texCoords = {
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f)
	};

	IDrawable* mesh = new Mesh(vertices, indices, "Do you like me.jpg", texCoords);
	

	int spriteAmount = 80;
	int frames = 100'000;
	int gridSize = 10;

	for (int i = 0; i < spriteAmount; i++)
	{
		meshes[i] = mesh->clone();
		rects[i] = new Rect(glm::vec2((((i % gridSize) / (float)gridSize) * 2.0) - 1.0, (((i / gridSize) / (float)gridSize) * 2.0) - 1.0), glm::vec2(0.2, 0.2), "sample_textures/tex (" + std::to_string(i + 1) + ").jpg", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	for (int k = 0; k < spriteAmount; k++)
	{
		renderer->loadTexture("sample_textures/tex (" + std::to_string(k + 1) + ").jpg");
	}

	renderer->loadTexture("Do you like me.jpg");

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < frames; i++)
	{
		for (int k = 0; k < spriteAmount; k++)
		{
			rects[k]->setTranslation(rects[k]->getTranslation() + glm::vec2(rand() % 10 * 0.0001f - 0.00045f, rand() % 10 * 0.0001f - 0.00045f));
			rects[k]->setScale(rects[k]->getScale() + glm::vec2(rand() % 10 * 0.0001f - 0.00045f, rand() % 10 * 0.0001f - 0.00045f));
			rects[k]->setColor(rects[k]->getColor() + glm::vec4(rand() % 10 * 0.001f - 0.0045f, rand() % 10 * 0.001f - 0.0045f, rand() % 10 * 0.001f - 0.0045f, 0));
			renderer->draw(rects[k]);
			meshes[k]->setTranslation(meshes[k]->getTranslation() + glm::vec2(rand() % 10 * 0.0001f - 0.00045f, rand() % 10 * 0.0001f - 0.00045f));
			meshes[k]->setScale(meshes[k]->getScale() + glm::vec2(rand() % 10 * 0.0001f - 0.00045f, rand() % 10 * 0.0001f - 0.00045f));
			meshes[k]->setColor(meshes[k]->getColor() + glm::vec4(rand() % 10 * 0.001f - 0.0045f, rand() % 10 * 0.001f - 0.0045f, rand() % 10 * 0.001f - 0.0045f, 0));
			renderer->draw(meshes[k]);
		}
		renderer->Display();
	}
	auto stop = std::chrono::high_resolution_clock::now();
	int duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

	std::cout << (float)frames / (duration * 0.001f) << "fps" << std::endl;

	return 0;
}