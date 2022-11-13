#include "Object.h"

Object::Object()
{
	m_Mesh = std::shared_ptr<Mesh>(new Rectangle());

	m_Position = glm::vec2(0.0f, 0.0f);
	m_Scale = glm::vec2(1.0f, 1.0f);

	m_ModelLocation = 0;
	m_ModelMatrix = glm::mat4(1.0f);
}

Object::Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color)
{
	m_Mesh = mesh;
	m_Position = position;
	m_Scale = scale;
	m_ModelLocation = modelLocation;
	m_ModelMatrix = glm::mat4(1.0f);

	m_ColorLocation = glGetUniformLocation(shaderID, "vertColor");
	m_Color = color;
}

void Object::Render()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Position, 0.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 0.0f));

	glUniformMatrix4fv(m_ModelLocation, 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));
	glUniform4fv(m_ColorLocation, 1, glm::value_ptr(m_Color));

	m_Mesh->Render();
}

Object::~Object()
{
}
