#include "Paddle.h"

Paddle::Paddle()
	: Object()
{
	m_MovementSpeed = 1.0f;
}

Paddle::Paddle(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLfloat movementSpeed, GLint bufferHeight, GLint bufferWidth, GLuint shaderID, glm::vec4 color, 
	GLfloat maxSpeed) :
	Object(mesh, position, scale, modelLocation, shaderID, color)
{
	m_InitialMovementSpeed = movementSpeed;
	m_MovementSpeed = movementSpeed;
	m_MaxSpeed = maxSpeed;
	m_BufferHeight = bufferHeight;
	m_BufferWidth = bufferWidth;
}

void Paddle::Reset()
{
	m_MovementSpeed = m_InitialMovementSpeed;
}

void Paddle::KeyMove(bool* keys, GLfloat& deltaTime, GLenum& keyUp, GLenum& keyDown)
{
	m_Velocity = m_MovementSpeed * deltaTime;

	if (keys[keyUp])
	{
		m_Position += glm::vec2(-1.0f, 0.0f) * m_Velocity;
	}

	if (keys[keyDown])
	{
		m_Position += glm::vec2(1.0f, 0.0f) * m_Velocity;
	}

	float barrierRatio = 1.75f;

	if (m_Position.x > (((float) m_BufferWidth / barrierRatio) - m_Scale.x))
		m_Position.x = (((float) m_BufferWidth / barrierRatio) - m_Scale.x);
												 
	if (m_Position.x < ((-(float) m_BufferWidth / barrierRatio) + m_Scale.x))
		m_Position.x = ((-(float) m_BufferWidth / barrierRatio) + m_Scale.x);
}

void Paddle::IncrSpeed(float speedIncr)
{
	m_MovementSpeed += speedIncr;

	if (m_MovementSpeed > m_MaxSpeed)
		m_MovementSpeed = m_MaxSpeed;
}

Paddle::~Paddle()
{
}
