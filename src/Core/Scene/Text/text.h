#pragma once

#include "../drawable.h"
#include "font.h"

struct RenderedCharacter {
	Vertex vertices[6];
	GLuint texture;
};

class Text : public Drawable {
public:
	Text(std::string font_path, GLuint size);
	Text(Font font);

	void draw(DrawType drawType = DrawType::TRIANGLEG) 						 override;

	void drawInstancing(int amount, DrawType drawType = DrawType::TRIANGLEG) override;

	///set active char index and get char texture for renderer 
	GLuint activateChar(int i = 0);

	Size getSize();

	void setText(std::string text);
	std::string getText();
protected:
	std::vector<Vertex> vertices;
	Font font;
	glm::vec4 color = glm::vec4(1.0f);
	int activeChar = 0;
	std::string text = "";
	std::vector<RenderedCharacter> renderedCharacters;
	GLuint VAO, VBO;

	virtual void setup();
};