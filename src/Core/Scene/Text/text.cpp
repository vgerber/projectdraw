#include "text.h"

Text::Text(std::string font_path, GLuint size)
{
	font = Font(font_path.c_str(), size);
	shader = Shaders[SHADER_FONT];

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::draw()
{
	glDisable(GL_CULL_FACE);
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	glUniform3f(glGetUniformLocation(shader.getId(), "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	GLfloat x = 0.0f, y = 0.0f; // position.x, y = position.y;
	for (auto c = text.begin(); c != text.end(); c++) {
		Character ch = font.characters[*c];

		/*GLfloat xpos = position.x + ch.Bearing.x * scale;
		GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;*/
		GLfloat xpos = x + ch.Bearing.x;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y);
		GLfloat w = ch.Size.x;
		GLfloat h = ch.Size.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h, 0.0, 0.0 },
			{ xpos,     ypos,     0.0, 1.0 },
			{ xpos + w, ypos,     1.0, 1.0 },
			{ xpos,     ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos,     1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is numberof 1 / 64 pixels)
		x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)

		if (reloadText) {
			if (size.width < xpos + w) {
				size.width = xpos + w;
			}
			if (size.height < ypos + h) {
				size.height = ypos + h;
			}
			if (c + 1 == text.end()) {
				loadBox();
				reloadText = false;

			}
		}
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_CULL_FACE);
}

void Text::setColor(glm::vec4 color)
{
	this->color = color;
}

void Text::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	color = glm::vec4(r, g, b, a);
}

void Text::setText(std::string text)
{
	if (this->text != text) {
		reloadText = true;
		size.x = 0.0f;
		size.y = 0.0f;
		size.z = 0.0f;
		size.width = 0.0f;
		size.height = 0.0f;
		size.depth = 0.0f;
	}

	this->text = text;

}

std::string Text::getText()
{
	return text;
}
