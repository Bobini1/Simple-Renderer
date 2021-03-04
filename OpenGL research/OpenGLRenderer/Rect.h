#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../IDrawable.h"

class Rect :
    public IDrawable
{
private:
	static unsigned int indices, vertices;
	std::string texPath;
	std::vector<glm::vec2> vertexData;
	static std::vector<unsigned int> elementData;
	static std::vector<glm::vec2> texCoordsData;
	glm::vec2 translation, scale;
	glm::vec3 rotation;
	glm::vec2 centerPoint;
	glm::vec4 color;
public:
	virtual glm::vec2 getCenterPoint() override;
	virtual IDrawable* clone() override;
	Rect(glm::vec2 origin, glm::vec2 size, std::string texPath = "color.png", glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	~Rect();
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
	/*virtual glm::vec3 getRotation() override;
	virtual void setRotation(glm::mat4 rotation) override;*/
};

