#include "window.h"

Window::Window(WindowInfo info, int width, int height, std::string title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, info.maximized);
	glfwWindowHint(GLFW_RESIZABLE, info.resizable);
	glfwWindowHint(GLFW_SAMPLES, info.samples);

	window = glfwCreateWindow(width, height, "Test", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	activate();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, width, height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	this->wInfo = info;
}

void Window::activate()
{
	glfwMakeContextCurrent(window);
}

GLFWwindow * Window::getWindow()
{
	return window;
}

Size Window::getSize()
{
	int width = 0;
	int height = 0;

	glfwGetWindowSize(window, &width, &height);
	Size size;
	size.width = width;
	size.height = height;
	return size;
}

Point Window::getCursorPosition()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Point{ glm::vec3(x, y, 0.0) };
}

void Window::update()
{
	
	if (!wInfo.cursorLeave) {
		Size windowSize = getSize();
		glm::vec3 mousePos = getCursorPosition().position;
		
		glm::vec2 newCursorPos(0.0f, 0.0f);
		if (mousePos.x < 0.0f) {
			mousePos.x = 0.0f;
		}
		if (mousePos.y < 0.0f) {
			mousePos.y = 0.0f;
		}

		if (mousePos.x > windowSize.width) {
			mousePos.x = windowSize.width;
		}
		if (mousePos.y > windowSize.height) {
			mousePos.y = windowSize.height;
		}
		glfwSetCursorPos(window, mousePos.x, mousePos.y);

	}
	glfwPollEvents();
}
