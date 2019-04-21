#pragma once

#include "../drawable.h"
#include "font.h"

struct RenderedCharacter {
	Vertex vertices[6];
	GLuint texture;
};

class Text : public Drawable {
public:
	/**
	 * @brief Construct a new Text object
	 * 
	 * @param font_path src path to *.ttf file
	 * @param size font size
	 */
	Text(std::string font_path, GLuint size);

	/**
	 * @brief Construct a new Text object
	 * 
	 * @param font generated font
	 */
	Text(Font font);

	/**
	 * @brief Get the text font
	 * 
	 * @return Font 
	 */
	Font getFont();

	/**
	 * @brief Get the Size of the text
	 * 
	 * @return Size 
	 */
	Size getSize();

	/**
	 * @brief Set the text
	 * 
	 * @param text 
	 */
	void setText(std::string text);

	/**
	 * @brief Get the text
	 * 
	 * @return std::string 
	 */
	std::string getText();
protected:
	Font font;
	std::string text = "";
	

	virtual void setup();

	virtual void callUpdate() override;
};