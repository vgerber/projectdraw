#pragma once

#include "../core.h"
#include "../Model/box.h"

struct WindowInfo {
	int major = 3;
	int minor = 3;

	bool maximized = false;
	bool resizable = false;

	int samples = 4;
};

class Window {
public:
	Window(WindowInfo info, int width, int height, std::string title);
	void activate();

	GLFWwindow *getWindow();
	Size getSize();

private:
	GLFWwindow *window;
};