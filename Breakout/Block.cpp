#include "Block.h"

Block::Block()
	: Object()
{
}

Block::Block(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color, bool render)
	: Object(mesh, position, scale, modelLocation, shaderID, color)
{
	m_RenderVal = render;
}

void Block::Update(std::shared_ptr<Ball> ball, GLfloat& deltaTime, unsigned int& numOfBlocks)
{
	if (m_Position.x - (m_Scale.x / 2) < ball->GetPosition().x + (ball->GetScale().x / 2) &&
		m_Position.x + (m_Scale.x / 2) > ball->GetPosition().x - (ball->GetScale().x / 2) &&
		m_Position.y - (m_Scale.y / 2) < ball->GetPosition().y + (ball->GetScale().y / 2) &&
		m_Position.y + (m_Scale.y / 2) > ball->GetPosition().y - (ball->GetScale().y / 2))
	{
		m_RenderVal = false;
		
		numOfBlocks--;

		if (ball->GetPosition().x > m_Position.x + (m_Scale.x / 2) || ball->GetPosition().x < m_Position.x - (m_Scale.x / 2))
		{
			ball->ReverseDirection(deltaTime, glm::vec2(-1.0f, 1.0f));
			return;
		}

		ball->ReverseDirection(deltaTime, glm::vec2(1.0f, -1.0f));
	}
}
