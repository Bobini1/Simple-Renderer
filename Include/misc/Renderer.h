#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include <misc/stb_image.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"



class Renderer
{
public:
	GLFWwindow* window;
	void CreateWindow(int width, int height)
	{
		// initial settings
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
		glViewport(0, 0, width, height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		stbi_set_flip_vertically_on_load(true);

		Shader SimplestShader("C:/Users/PC/source/GLSL/Simplest.vs", "C:/Users/PC/source/GLSL/Simplest.fs");
	}
	unsigned int AddTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		glm::vec2 vertices = { p1, p2, p3 };
		return generate(vertices);
	}
	unsigned int AddSquare(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4)
	{
		glm::vec2 vertices = { p1, p2, p3, p3, p4, p1 };
		return generate(vertices);
	}
	void DrawSquare(unsigned int ID)
	{
		SimplestShader.use();
		glBindVertexArray(ID);
		glDrawArray(GL_TRIANGLES, 0, 6);
	}
	void DrawTriangle(unsigned int ID)
	{
		SimplestShader.use();
		glBindVertexArray(ID);
		glDrawArray(GL_TRIANGLES, 0, 3);
	}
	void Display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

private:
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	unsigned int generate(glm::vec2 vertices[])
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		return VAO;
	}
};
