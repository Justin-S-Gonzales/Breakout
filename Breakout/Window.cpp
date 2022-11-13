#include "Window.h"

Window::Window()
{
	m_Width = 640, m_Height = 480;
	m_BufferWidth = 0, m_BufferHeight = 0;
}

Window::Window(unsigned int width, unsigned int height)
{
	m_Width = width, m_Height = height;
	m_BufferWidth = 0, m_BufferHeight = 0;
}

bool Window::CreateWindow()
{
	// Init glfw
	if (!glfwInit())
	{
		std::cout << "GLFW INIT FAILED." << std::endl;
		glfwTerminate();
		return false;
	}

	// Setup GLFW Window hints
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glewExperimental = GL_TRUE;

	m_Window = glfwCreateWindow(m_Width, m_Height, "Breakout", nullptr, nullptr);

	if (!m_Window)
	{
		std::cout << "Error creating GLFW window!" << std::endl;
		glfwTerminate();
		return false;
	}

	// Get buffer size information
	glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);

	// Set the current context
	glfwMakeContextCurrent(m_Window);
	
	// Create callbacks
	CreateCallback();

	// Init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return false;
	}

	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	glEnable(GL_BLEND);

	glfwSetWindowUserPointer(m_Window, this);
}

void Window::CreateCallback()
{
	glfwSetKeyCallback(m_Window, handleKeys);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->m_Keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->m_Keys[key] = false;
		}
	}
}



Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}