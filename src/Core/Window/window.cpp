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
