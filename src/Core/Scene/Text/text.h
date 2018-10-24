#pragma once

#include "../drawable.h"
#include "font.h"

class Text : public Drawable {
public:
	Text(std::string font_path, GLuint size);

	void draw();

	void setColor(glm::vec4 color);
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	void setText(std::string text);
	std::string getText();
protected:
	Font font;
	glm::vec4 color = glm::vec4(1.0f);
	std::string text = "";
private:
	GLuint VAO, VBO;
	bool reloadText = false;
};