#pragma once

#include "Rectangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <memory>

class Object
{
protected:
	std::shared_ptr<Mesh> m_Mesh;

	glm::vec2 m_Position;
	glm::vec2 m_Scale;

	GLuint m_ModelLocation;
	glm::mat4 m_ModelMatrix;

	glm::vec4 m_Color;
	GLuint m_ColorLocation;

public:
	Object();
	Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color);

	void Render();

	glm::vec2 GetPosition() { return m_Position; }
	glm::vec2 GetScale() { return m_Scale; }

	void Move(glm::vec2 trajectory) { m_Position += trajectory; }

	~Object();
};

