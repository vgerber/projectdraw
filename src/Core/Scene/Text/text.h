#pragma once

#include "../drawable.h"
#include "font.h"

class Text : public Drawable {
public:
	Text(std::string font_path, GLuint size);

	void draw();

	void set_color(glm::vec4 color);
	void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	void set_text(std::string text);
	std::string get_text();
protected:
	Font font;
	glm::vec4 color = glm::vec4(1.0f);
	std::string text = "";
private:
	GLuint VAO, VBO;
	bool reload_text = false;
};