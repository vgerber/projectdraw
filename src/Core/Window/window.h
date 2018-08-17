#pragma once

#include "../core.h"
#include "../Model/box.h"
#include "../Scene/Primitive/geometry.h"


struct WindowInfo {
	int major = 3;
	int minor = 3;

	bool maximized = false;
	bool resizable = false;

	bool cursorLeave = false;

	int samples = 4;
};


class Window {
public:
	Window(WindowInfo info, int width, int height, std::string title);
	void activate();


	sf::Window* getWindow();
	Size getSize();
	
	Point getCursorPosition();


	void update();

private:
	WindowInfo wInfo;
	sf::Window *window;
	Point oldMousePosition;
};