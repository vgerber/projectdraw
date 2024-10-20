#include "window.h"

Window::Window(WindowInfo info, int width, int height, std::string title)
{

	sf::ContextSettings settings;

	settings.depthBits = 24;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = info.major;
	settings.minorVersion = info.minor;

	sf::Uint32 style = sf::Style::None;
	if (info.resizable)
		style |= sf::Style::Resize;
	if (info.maximized)
		style |= sf::Style::Fullscreen;
	if (info.titlebar)
		style |= sf::Style::Titlebar;
	if (info.closeButton)
		style |= sf::Style::Close;



	window = new sf::Window(sf::VideoMode(width, height), title, style, settings);
	
	activate();

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, width, height);

	int winX, winY;

	//window->setMouseCursorGrabbed(!wInfo.cursorLeave);

	this->wInfo = info;

	oldMousePosition = getCursorPosition();
}

void Window::activate()
{
	window->setActive(true);
}

sf::Window* Window::getWindow()
{
	return window;
}

Size Window::getSize()
{
	int width = window->getSize().x;
	int height = window->getSize().y;

	Size size;
	size.width = width;
	size.height = height;
	return size;
}

Point Window::getCursorPosition()
{
	double x = 0.0, y = 0.0;
	sf::Vector2i mPos = sf::Mouse::getPosition(*window);
	x = mPos.x;
	y = mPos.y;
	return Point{ glm::vec3(x, y, 0.0) };
}

void Window::update()
{
}
