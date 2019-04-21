#include "font.h"

Font::Font()
{
}

Font::Font(const char * file, GLuint size)
{
	load(file, size);
}

glm::ivec2 Font::getVerticalBounds() {
	return verticalBounds;
}

void Font::load(const char * file, GLuint size)
{
	FT_Library ft;
	FT_Face face;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error::FreeType Could not load" << std::endl;
	}
	if (FT_New_Face(ft, file, 0, &face)) {
		std::cout << "Error::FreeType Failed to load VeraMono.ttf" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Error::FreeType Failed to load Glyph" << std::endl;
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0, 
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};

		verticalBounds.x = std::max(face->glyph->bitmap_top, verticalBounds.x);
		verticalBounds.y = std::min((int)face->glyph->bitmap.rows-face->glyph->bitmap_top, verticalBounds.y);		

		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
