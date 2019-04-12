#include "text.h"

Text::Text(std::string font_path, GLuint size)
{
	font = Font(font_path.c_str(), size);

	setup();
}

Text::Text(Font font) {
	this->font = font;
	setup();
}

void Text::draw(DrawType drawType)
{

}

Font Text::getFont() {
	return font;
}

Size Text::getSize() {
	return size;
}

void Text::setText(std::string text)
{
	if (this->text != text) {
		this->text = text;
		callUpdate();
	}
}

std::string Text::getText()
{
	return text;
}

void Text::setup() {
}
