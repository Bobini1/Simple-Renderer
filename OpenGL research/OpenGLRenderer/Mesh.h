#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../IDrawable.h"

class Mesh :public IDrawable
{
private:
	std::vector<glm::vec2> vertexData;
	std::vector<unsigned int> elementData;

	std::string texPath;
	std::vector<glm::vec2> texCoordsData;
	glm::vec4 color;

	glm::vec2 translation, scale;
	glm::vec3 rotation;

	glm::vec2 centerPoint;

public:
	virtual glm::vec2 getCenterPoint() override;
	virtual IDrawable* clone() override;
	Mesh(std::vector<glm::vec2> vertexPositions = {}, std::vector<unsigned int> elements = {}, std::string texPath = "color.png", std::vector<glm::vec2> texCoords = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	~Mesh();
	virtual std::string getTexPath() override;
	virtual std::vector<unsigned int> getElementData() override;
	virtual std::vector<glm::vec2> getVertexData() override;
	virtual std::vector<glm::vec2> getTexCoordsData() override;
	virtual glm::vec4 getColor() override;
	virtual void setColor(glm::vec4 color) override;
	virtual glm::vec2 getTranslation() override;
	virtual void setTranslation(glm::vec2 translation) override;
	virtual glm::vec2 getScale() override;
	virtual void setScale(glm::vec2 scale) override;

	// unimplemented
	/*virtual glm::vec3 getRotation() override;
	virtual void setRotation(glm::mat4 rotation) override;*/
};
