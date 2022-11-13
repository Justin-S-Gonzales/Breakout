#pragma once
#include "Object.h"

#include "Paddle.h"

#include <cstdlib>
#include <time.h>
#include <iostream>

class Ball :
    public Object
{
public:
    Ball();
    Ball(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, int bufferWidth, int bufferHeight, int movementSpeed, GLuint shaderID, glm::vec4 color, 
        int maxSpeed);

    void Start();
    void Update(std::shared_ptr<Paddle> paddle, GLfloat& deltaTime, unsigned int& lives);

    void ReverseDirection(GLfloat& deltaTime, glm::vec2 direction);

    ~Ball();

private:
    int m_BufferWidth, m_BufferHeight;
    int m_InitialMovementSpeed;
    int m_MovementSpeed;
    int m_MaxSpeed;
    int m_Bounce;
    int m_MaxBounce;
    float m_Counter;

    unsigned int m_RainbowColor;

    glm::vec2 m_Direction;

    void SetRandomBounce();
};

