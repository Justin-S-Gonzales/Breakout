#include "Ball.h"

Ball::Ball() : Object()
{
	m_BufferWidth = 0;
	m_BufferHeight = 0;
	m_InitialMovementSpeed = 0;
	m_MaxSpeed = 0;
	m_MovementSpeed = 0;
	m_Bounce = 0;
	m_MaxBounce = 0;
}

Ball::Ball(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, int bufferWidth, int bufferHeight, int movementSpeed, GLuint shaderID, glm::vec4 color, int maxSpeed)
	: Object(mesh, position, scale, modelLocation, shaderID, color)
{
	m_BufferWidth = bufferWidth;
	m_BufferHeight = bufferHeight;
	m_InitialMovementSpeed = movementSpeed;
	m_MaxSpeed = maxSpeed;
	m_MovementSpeed = m_MovementSpeed;
	m_Bounce = 20 * m_MovementSpeed;
	m_MaxBounce = 20 * m_MaxSpeed;
}

void Ball::Start()
{
	srand(time(NULL));

	float randNum1 = (float) (rand() % 100) + 1;
	float randNum2 = (float) (rand() % 100) + 1;

	if (rand() % 10 > 5)
	{
		randNum1 = -randNum1;
	}

	if (rand() % 10 > 5)
	{
		randNum2 = -randNum2;
	}

	m_MovementSpeed = m_InitialMovementSpeed;

	m_Position = glm::vec2(0.0f, 0.0f);

	m_Direction = glm::vec2(randNum1, randNum2);

	if (abs(m_Direction.y) > abs(m_Direction.x))
	{
		float temp = m_Direction.y;
		m_Direction.x = temp;
		m_Direction.y = m_Direction.x;
	}

	m_Direction = glm::normalize(m_Direction);

	m_Bounce = m_InitialMovementSpeed * 2;
	m_MaxBounce = m_MaxSpeed * 2;

	m_RainbowColor = 0;
}

void Ball::SetRandomBounce()
{
	// Bounces off in random direction
	if (m_Direction.x > 0)
		m_Direction.x += (100.0f / (rand() % 100)) + 3.0f;
	if (m_Direction.x < 0)
		m_Direction.x -= (100.0f / (rand() % 100)) + 3.0f;

	if (m_Direction.y > 0)
		m_Direction.y += (100.0f / (rand() % 100)) + 3.0f;
	if (m_Direction.y < 0)
		m_Direction.y -= (100.0f / (rand() % 100)) + 3.0f;

	// Add a little bit of speed to make more difficult to return
	m_MovementSpeed += 20;

	if (m_MovementSpeed > m_MaxSpeed)
		m_MovementSpeed = m_MaxSpeed;
	 
	m_Bounce += 40;

	if (m_Bounce > m_MaxBounce)
		m_Bounce = m_MaxBounce;
}

void Ball::Update(std::shared_ptr<Paddle> paddle, GLfloat& deltaTime, unsigned int& lives)
{
	// Move position by direction
	m_Direction = glm::normalize(m_Direction);

	if (m_Direction.x == 0.0f || m_Direction.y == 0.0f)
		m_Direction = glm::vec2(0.9f, 0.4f);

	m_Position += m_Direction * deltaTime * (float) m_MovementSpeed;


	// If it hits walls
	if (m_Position.x + (m_Scale.x / 2) > m_BufferWidth / 2)
	{
		m_Direction.x = -m_Direction.x;
		paddle->IncrSpeed(20.0f);
		SetRandomBounce();
		m_Position.x -= m_Bounce * deltaTime;
	}

	if (m_Position.x - (m_Scale.x / 2) < -m_BufferWidth / 2)
	{
		m_Direction.x = -m_Direction.x;
		paddle->IncrSpeed(20.0f);
		SetRandomBounce();
		m_Position.x += m_Bounce * deltaTime;
	}

	// It hits the ceiling
	if (m_Position.y + (m_Scale.y / 2) > m_BufferHeight / 2)
	{
		m_Direction.y = -m_Direction.y;
		paddle->IncrSpeed(20.0f);
		SetRandomBounce();
		m_Position.y -= m_Bounce * deltaTime;
	}

	// If it gets past paddle
	if (m_Position.y - (m_Scale.y / 2) < -m_BufferHeight)
	{
		paddle->Reset();
		Start();
		lives--;
		if (lives == 1)
			std::cout << "You have " << lives << " life." << std::endl;
		else 
			std::cout << "You have " << lives << " lives." << std::endl;
		return;
	}

	// Paddle 1 bounces it back
	if (
		(m_Position.x - (m_Scale.x / 2)) < paddle->GetPosition().x + (paddle->GetScale().x / 2) &&
		(m_Position.x + (m_Scale.x / 2)) > paddle->GetPosition().x - (paddle->GetScale().x / 2) &&
		(m_Position.y - (m_Scale.y / 2)) < paddle->GetPosition().y + (paddle->GetScale().y / 2) &&
		(m_Position.y + (m_Scale.y / 2)) > paddle->GetPosition().y - (paddle->GetScale().y / 2)
	)
	{
		m_Direction.y = -m_Direction.y;
		paddle->IncrSpeed(20.0f);
		SetRandomBounce();
		m_Position.y += m_Bounce * deltaTime;
	}

	// Rainbow effect
	if (m_RainbowColor == 0)
		m_Color -= glm::vec4(0.2f * deltaTime, 0.0f, 0.0f, 0.0f);
	else if (m_RainbowColor == 1)
		m_Color -= glm::vec4(0.0f, 0.2f * deltaTime, 0.0f, 0.0f);
	else if (m_RainbowColor == 2)
		m_Color -= glm::vec4(0.0f, 0.0f, 0.2f * deltaTime, 0.0f);
	else if (m_RainbowColor == 3)
		m_Color += glm::vec4(0.2f * deltaTime, 0.0f, 0.0f, 0.0f);
	else if (m_RainbowColor == 4)
		m_Color += glm::vec4(0.0f, 0.2f * deltaTime, 0.0f, 0.0f);
	else if (m_RainbowColor == 5)
		m_Color += glm::vec4(0.0f, 0.0f, 0.2f * deltaTime, 0.0f);

	if (m_Color.x < 0.2)
		m_RainbowColor = 1;
	if (m_Color.y < 0.5)
		m_RainbowColor = 2;
	if (m_Color.z < 0.2)
		m_RainbowColor = 3;
	if (m_Color.x > 1.0)
		m_RainbowColor = 4;
	if (m_Color.y > 1.0)
		m_RainbowColor = 5;
	if (m_Color.z > 1.0)
		m_RainbowColor = 6;

	if (m_RainbowColor == 6)
	{
		m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_RainbowColor = 0;
	}
}

void Ball::ReverseDirection(GLfloat& deltaTime, glm::vec2 direction)
{
	m_Direction *= direction;

	if (m_Direction.x > 0)
		m_Position.x += m_Bounce * deltaTime;
	else if (m_Direction.x < 0)
		m_Position.x -= m_Bounce * deltaTime;

	if (m_Direction.y > 0)
		m_Position.y += m_Bounce * deltaTime;
	else if (m_Direction.y < 0)
		m_Position.y -= m_Bounce * deltaTime;

	SetRandomBounce();
}

Ball::~Ball()
{
}
