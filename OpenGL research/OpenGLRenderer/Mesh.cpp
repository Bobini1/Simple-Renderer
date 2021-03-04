#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec2> vertexPositions, std::vector<unsigned int> elements, std::string texPath, std::vector<glm::vec2> texCoords, glm::vec4 color)
	:texPath(texPath), vertexData(vertexPositions), elementData(elements), texCoordsData(texCoords), color(color)
{
	translation = glm::vec2(0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec2(1.0f, 1.0f);

	centerPoint = glm::vec2(0.0f, 0.0f);
	for (auto& position : vertexPositions)
	{
		centerPoint += position;
	}
	centerPoint /= vertexPositions.size();
}

glm::vec2 Mesh::getCenterPoint()
{
	return centerPoint;
}

std::vector<unsigned int> Mesh::getElementData()
{
	return elementData;
}

std::vector<glm::vec2> Mesh::getVertexData()
{
	return vertexData;
}

std::vector<glm::vec2> Mesh::getTexCoordsData()
{
	return texCoordsData;
}

glm::vec2 Mesh::getTranslation()
{
	return translation;
}

void Mesh::setTranslation(glm::vec2 translation)
{
	this->translation = translation;
}

glm::vec2 Mesh::getScale()
{
	return scale;
}

void Mesh::setScale(glm::vec2 scale)
{
	this->scale = scale;
}

/*glm::vec3 Mesh::getRotation()
{
	return rotation;
}

void Mesh::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}*/

IDrawable* Mesh::clone()
{
	return new Mesh(*this);
}

Mesh::~Mesh()
{
}

std::string Mesh::getTexPath()
{
	return texPath;
}


glm::vec4 Mesh::getColor()
{
	return color;
}

void Mesh::setColor(glm::vec4 color)
{
	this->color = color;
}