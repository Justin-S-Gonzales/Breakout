#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// For fonts
#include "ft2build.h"
#include FT_FREETYPE_H  

#include <iostream>
#include <vector>
#include <memory>

#include "Window.h"
#include "Shader.h"
#include "Rectangle.h"
#include "Object.h"
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"

// Todo: Add 10 second loading
// Todo: Add lives counter

void playLevel();

// Delta time
GLfloat now;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Window window;

std::shared_ptr<Rectangle> rect;

std::shared_ptr<Ball> ball;
std::shared_ptr<Paddle> paddle;

unsigned int numOfBlocks;
std::vector<std::shared_ptr<Block>> blocks;

unsigned int level = 0;
unsigned int lives = 3;

GLenum keyUp = GLFW_KEY_A;
GLenum keyDown = GLFW_KEY_D;

int main()
{
	float width = 1000;
	float height = 720;

	// Window
	window = Window(width, height);
	window.CreateWindow();

	// Shader
	Shader shader = Shader();
	shader.CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
	shader.UseShader();
	shader.Validate();

	// Projection matrix uniform
	glm::mat4 projection = glm::ortho((GLfloat)(-(window.GetBufferWidth() / 2)), (GLfloat)(window.GetBufferWidth() / 2),
		(GLfloat)(-(window.GetBufferHeight() / 2)), (GLfloat)(window.GetBufferHeight() / 2));

	GLuint uProjection = shader.GetProjectionLocation();

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

	// Get model matrix location
	GLuint uModel = shader.GetModelLocation();

	rect = std::shared_ptr<Rectangle>(new Rectangle());

	float positionRatio = 2.15f;
	float scaleRatio = 5.0f;

	paddle = std::shared_ptr<Paddle>(new Paddle(rect, glm::vec2(0.0f, -height / positionRatio), glm::vec2(height / scaleRatio, 20.0f), uModel, 300.0f, window.GetBufferHeight(), window.GetBufferWidth(),
		shader.GetShaderID(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 540.0f));;
	ball = std::shared_ptr<Ball>(new Ball(rect, glm::vec2(0.0f, 0.0f), glm::vec2(15.0f, 15.0f), uModel, window.GetBufferWidth(), window.GetBufferHeight(), 220.0f,
		shader.GetShaderID(), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 600.0f));

	std::shared_ptr<Block> block(new Block(rect,  glm::vec2(0.0f, 300.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(1.0f, 0.0f, 0.1f, 1.0f), true));
	std::shared_ptr<Block> block2(new Block(rect, glm::vec2(0.0f, 275.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(0.75f, 0.0f, 0.2f, 1.0f), true));
	std::shared_ptr<Block> block3(new Block(rect, glm::vec2(0.0f, 250.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(0.5f, 0.0f, 0.3f, 1.0f), true));

	std::shared_ptr<Block> block4(new Block(rect, glm::vec2(0.0f, 225.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(0.05f, 1.0f, 0.05f, 1.0f), true));
	std::shared_ptr<Block> block5(new Block(rect, glm::vec2(0.0f, 200.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(0.1f, 0.75f, 0.1f, 1.0f), true));
	std::shared_ptr<Block> block6(new Block(rect, glm::vec2(0.0f, 175.0f), glm::vec2(50.0f, 25.0f), uModel, shader.GetShaderID(), glm::vec4(0.15f, 0.5f, 0.15f, 1.0f), true));

	std::cout << "You have " << lives << " lives." << std::endl;
	std::cout << "Level " << level + 1 << "." << std::endl;

	// Main loop
	window.GetKeys()[GLFW_KEY_R] = false;
	window.GetKeys()[keyUp] = false;
	window.GetKeys()[keyDown] = false;
	if (level == 0)
	{
		for (float i = (-window.GetBufferWidth() / 2) + (block->GetScale().x / 2); i < (window.GetBufferWidth() / 2); i += block->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block2->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block2->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block2)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block3->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block3->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block3)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		ball->Start();
		paddle->Reset();
		while (level == 0 && !window.GetShouldClose())
		{ 
			playLevel();

			if (lives < 1)
			{
				std::cout << "GAME OVER" << std::endl;
				return 0;
			}

			if (numOfBlocks < 10)
			{
				level = 1;
				std::cout << "Level " << level + 1 << "." << std::endl;
				lives++;
				if (lives == 1)
					std::cout << "You have " << lives << " life." << std::endl;
				else
					std::cout << "You have " << lives << " lives." << std::endl;
			}
		}
	}

	window.GetKeys()[GLFW_KEY_R] = false;
	window.GetKeys()[keyUp] = false;
	window.GetKeys()[keyDown] = false;

	if (level == 1)
	{
		for (float i = (-window.GetBufferWidth() / 2) + (block->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block2->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block2->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block2)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block3->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block3->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block3)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block4->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block4->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block4)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block5->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block5->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block5)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		for (float i = (-window.GetBufferWidth() / 2) + (block6->GetScale().x / 2); i < window.GetBufferWidth() / 2; i += block6->GetScale().x)
		{
			blocks.push_back(std::shared_ptr<Block>(new Block(*block6)));
			blocks.back()->Move(glm::vec3(i, 0.0f, 0.0f));
			numOfBlocks++;
		}

		ball->Start();
		paddle->Reset();
		while (level == 1 && !window.GetShouldClose())
		{
			playLevel();

			if (lives < 1)
			{
				std::cout << "GAME OVER" << std::endl;
				return 0;
			}

			if (numOfBlocks < 10)
				std::cout << "YOU WON!" << std::endl;
		}
	}
	
}

void playLevel()
{
	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	glfwPollEvents();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//if (window.GetKeys()[GLFW_KEY_R])
	//{
	//	ball->Start();
	//	paddle->Reset();
	//}

	ball->Update(paddle, deltaTime, lives);
	ball->Render();

	paddle->KeyMove(window.GetKeys(), deltaTime, keyUp, keyDown);
	paddle->Render();

	for (unsigned int i = 0; i < blocks.size(); i++)
		if (blocks[i]->GetRenderVal() == true) blocks[i]->Update(ball, deltaTime, numOfBlocks);

	for (unsigned int i = 0; i < blocks.size(); i++)
		if (blocks[i]->GetRenderVal() == true) blocks[i]->Render();

	window.SwapBuffers();
}