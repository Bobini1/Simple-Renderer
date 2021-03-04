#include "OpenGLRenderer.h"


struct OpenGLRenderer::Texture
{
	int texWidth;
	int texHeight;
	float layer;
};

OpenGLRenderer::OpenGLRenderer(unsigned int screenWidth, unsigned int screenHeight, std::string windowName, glm::vec4 backgroundColor)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	// initial settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), NULL, NULL);
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
	glfwSwapInterval(0);
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenVertexArrays(1, &mainVAO);
	glBindVertexArray(mainVAO);

	glGenBuffers(1, &mainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mainVBO);
	glBufferData(GL_ARRAY_BUFFER, VBOSize, nullptr, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &mainEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOSize, nullptr, GL_DYNAMIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);
	// tex coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	// tex layer attribute
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, layer));
	glEnableVertexAttribArray(3);

	simplestShader = new Shader("GLSL/Simplest.vs", "GLSL/Simplest.fs");
	simplestShader->use();

	vertices = new Vertex[400000];
	indices = new unsigned int[1000000];

	currentSpotInArrayTexture = 0.0f;
	regenMipmaps = false;
	TexturesInitialized = false;
	dynamicEBOoffset = 0, dynamicVBOoffset = 0;
	maxTexHeight = 0;
	maxTexWidth = 0.0f;
	arrayTexture = 0;
	textureSlots = 0;
	drawOrder = 0.9999f;
}

void OpenGLRenderer::TexInit(unsigned int maxTexWidth, unsigned int maxTexHeight, unsigned int numTextures)
{
	unsigned int numMipmaps = floor(log2(std::max(maxTexWidth, maxTexHeight))) + 1;
	// We want to increase it by one to make sure we have one white pixel layer for meshes without any texture.
	numTextures++;

	// Texture array
	glGenTextures(1, &arrayTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTexture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		numMipmaps,						//mipmaps + 1
		GL_RGBA8,                       //Internal format
		maxTexWidth, maxTexHeight,		//width,height
		numTextures						//Number of layers
	);

	// White pixel.
	loadTexture("color.png");

	// Everything looks nice with those parameters.
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	TexturesInitialized = true;

	this->maxTexWidth = maxTexWidth;
	this->maxTexHeight = maxTexHeight;
	textureSlots = numTextures;
	drawOrder = 0.9999f;
}

void OpenGLRenderer::drawRectangle(glm::vec2 origin, glm::vec2 size, glm::vec4 color)
{
	glm::vec2 p1 = origin;
	glm::vec2 p2;
	p2.x = p1.x;
	p2.y = p1.y + size.y;
	glm::vec2 p3;
	p3.x = p1.x + size.x;
	p3.y = p1.y + size.y;
	glm::vec2 p4;
	p4.x = p1.x + size.x;
	p4.y = p1.y;

	std::vector<glm::vec2> input = { p1, p2, p3, p4 };

	std::vector<unsigned int> indices = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
	};
	generate(input, indices, "color.png", texCoords, color);
}

void OpenGLRenderer::drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color)
{
	std::vector<glm::vec2> input = { p1, p2, p4, p3 };

	std::vector<unsigned int> indices = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
	};
	generate(input, indices, "color.png", texCoords, color);
}

void OpenGLRenderer::drawRectangle(glm::vec2 origin, glm::vec2 size, std::string texPath)
{
	glm::vec2 p1 = origin;
	glm::vec2 p2;
	p2.x = p1.x;
	p2.y = p1.y + size.y;
	glm::vec2 p3;
	p3.x = p1.x + size.x;
	p3.y = p1.y + size.y;
	glm::vec2 p4;
	p4.x = p1.x + size.x;
	p4.y = p1.y;

	
	std::vector<glm::vec2> input = { p1, p2, p3, p4 };

	std::vector<unsigned int> indices = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
	};

	generate(input, indices, texPath, texCoords, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void OpenGLRenderer::drawQuadrangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, std::string texPath)
{
	std::vector<glm::vec2> input = { p1, p2, p4, p3 };

	std::vector<unsigned int> indices = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	std::vector<glm::vec2> texCoords = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
	};

	generate(input, indices, texPath, texCoords, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void OpenGLRenderer::Display()
{
	if (!TexturesInitialized)
	{
		TexInit(1, 1, 0);
	}
	if (regenMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		regenMipmaps = false;
	}
	if (dynamicVBOoffset)
	{
		if (dynamicVBOoffset * 10 * sizeof(float) > VBOSize) std::cout << "Vertex buffer overflow." << std::endl;
		if (dynamicEBOoffset * sizeof(unsigned int) > EBOSize) std::cout << "Element buffer overflow." << std::endl;

		glBufferSubData(GL_ARRAY_BUFFER, 0, dynamicVBOoffset * 10 * sizeof(float), vertices);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dynamicEBOoffset * sizeof(unsigned int), indices);
		glDrawElements(GL_TRIANGLES, dynamicEBOoffset, GL_UNSIGNED_INT, (void*)0);
	}
	glfwSwapBuffers(window);
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	dynamicEBOoffset = 0;
	dynamicVBOoffset = 0;
	drawOrder = 0.999f;
}


void OpenGLRenderer::draw(IDrawable* drawable)
{
	// Scaling is done relative to center point for convenience.
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(drawable->getTranslation(), 0.0f)) * /* mesh->getRotation() * */ glm::translate(glm::mat4(1.0f), glm::vec3(drawable->getCenterPoint(), 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(drawable->getScale(), 0.0f)) * glm::translate(glm::mat4(1.0f), -glm::vec3(drawable->getCenterPoint(), 0.0f));
	std::vector<glm::vec2> tranformedVertices = drawable->getVertexData();
	if (transform != glm::mat4(1.0f))
	{
		for (auto& position : tranformedVertices)
		{
			position = (transform * glm::vec4(position, 0.0f, 1.0f));
		}
	}
	generate(tranformedVertices, drawable->getElementData(),  drawable->getTexPath(), drawable->getTexCoordsData(), drawable->getColor());
}


void OpenGLRenderer::loadTexture(std::string texPath)
{
	float layer;
	int width = 0;
	int height = 0;
	if (textures.find(texPath) == textures.end())
	{
		layer = addTex(texPath, &width, &height);
		textures[texPath].layer = layer;
		textures[texPath].texWidth = width;
		textures[texPath].texHeight = height;
		regenMipmaps = true;
	}
}

OpenGLRenderer::~OpenGLRenderer()
{
	delete vertices;
	delete indices;
	glDeleteBuffers(1, &mainEBO);
	glDeleteBuffers(1, &mainVBO);
	glDeleteVertexArrays(1, &mainVAO);
	glDeleteProgram(simplestShader->ID);
	glDeleteTextures(1, &arrayTexture);
	delete simplestShader;
}

void OpenGLRenderer::checkGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err;
	}
}

void OpenGLRenderer::generate(std::vector<glm::vec2> vertexPositions, std::vector<unsigned int> elements, std::string texPath, std::vector<glm::vec2> texCoords, glm::vec4 color)
{
	float layer = 0.0f;
	int width = 0;
	int height = 0;
	if (textures.find(texPath) == textures.end())
	{
		layer = addTex(texPath, &width, &height);
		textures[texPath].layer = layer;
		textures[texPath].texWidth = width;
		textures[texPath].texHeight = height;
		regenMipmaps = true;
	}
	else
	{
		layer = textures[texPath].layer;
		width = textures[texPath].texWidth;
		height = textures[texPath].texHeight;
	}
	for (unsigned int i = dynamicVBOoffset; i < dynamicVBOoffset + vertexPositions.size(); i++)
	{
		vertices[i].position = glm::vec3(vertexPositions[i - dynamicVBOoffset], drawOrder);
		vertices[i].color = color;
		vertices[i].texCoords = glm::vec2(texCoords[i - dynamicVBOoffset].x * width / maxTexWidth, texCoords[i - dynamicVBOoffset].y * height / maxTexHeight);
		vertices[i].layer = layer;
	}
	for (unsigned int i = dynamicEBOoffset; i < dynamicEBOoffset + elements.size(); i++)
	{
		indices[i] = elements[i - dynamicEBOoffset] + dynamicVBOoffset;
	}
	dynamicVBOoffset += vertexPositions.size();
	dynamicEBOoffset += elements.size();
	drawOrder -= zBufferResolution;
}

float OpenGLRenderer::addTex(std::string path, int* width, int* height)
{
	int nrChannels;
	unsigned char* data;
	data = stbi_load(path.c_str(), width, height, &nrChannels, 4);
	if (*width > maxTexWidth || *height > maxTexHeight) std::cout << "Picture too big. Assign bigger dimensions in texInit()" << std::endl;
	if (data)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,									  //Mipmap number
			0, 0, int(currentSpotInArrayTexture), //xoffset, yoffset, zoffset
			*width, *height, 1,					  //width, height, depth
			GL_RGBA,							  //format
			GL_UNSIGNED_BYTE,					  //type
			data);								  //pointer to data
		stbi_image_free(data);
		currentSpotInArrayTexture++;
	}
	return currentSpotInArrayTexture - 1;
}