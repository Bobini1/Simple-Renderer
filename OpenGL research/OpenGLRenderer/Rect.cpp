#include "Rect.h"


unsigned int Rect::indices = 6, Rect::vertices = 4;
std::vector<unsigned int> Rect::elementData = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
};
std::vector<glm::vec2> Rect::texCoordsData = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f)
};

Rect::Rect(glm::vec2 origin, glm::vec2 size, std::string texPath, glm::vec4 color)
	:texPath(texPath), color(color)
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

	vertexData = { p1, p2, p3, p4 };

	translation = glm::vec2(0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec2(1.0f, 1.0f);

	centerPoint = glm::vec2(0.0f, 0.0f);
	for (auto& position : vertexData)
	{
		centerPoint += position;
	}
	centerPoint /= vertices; //number of vertices
}

glm::vec2 Rect::getCenterPoint()
{
	return centerPoint;
}

std::vector<unsigned int> Rect::getElementData()
{
	return elementData;
}

std::vector<glm::vec2> Rect::getVertexData()
{
	return vertexData;
}

std::vector<glm::vec2> Rect::getTexCoordsData()
{
	return texCoordsData;
}

glm::vec2 Rect::getTranslation()
{
	return translation;
}

void Rect::setTranslation(glm::vec2 translation)
{
	this->translation = translation;
}

glm::vec2 Rect::getScale()
{
	return scale;
}

void Rect::setScale(glm::vec2 scale)
{
	this->scale = scale;
}

/*glm::vec3 Rect::getRotation()
{
	return rotation;
}

void Rect::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}*/

IDrawable* Rect::clone()
{
	return new Rect(*this);
}

Rect::~Rect()
{
}

std::string Rect::getTexPath()
{
	return texPath;
}


glm::vec4 Rect::getColor()
{
	return color;
}

void Rect::setColor(glm::vec4 color)
{
	this->color = color;
}