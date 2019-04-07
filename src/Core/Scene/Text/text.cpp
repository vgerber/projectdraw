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
	glDisable(GL_CULL_FACE);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, activeChar * 6, 6);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
}

GLuint Text::activateChar(int index) {
	activeChar = index;
	return renderedCharacters[activeChar].texture;
}

Size Text::getSize() {
	return size;
}

void Text::setText(std::string text)
{
	
	if (this->text != text) {
		renderedCharacters.resize(text.size());
		size.x = 0.0f;
		size.y = 0.0f;
		size.z = 0.0f;
		size.width = 0.0f;
		size.height = 0.0f;
		size.depth = 0.0f;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);


		if(this->text.size() < text.size()) {
			//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6 * text.size(), NULL, GL_DYNAMIC_DRAW);
		}	

		GLfloat x = 0.0f, y = 0.0f; // position.x, y = position.y;
		for (int i = 0; i < text.size(); i++) {
			Character ch = font.characters[text[i]];

			GLfloat xpos = x + ch.Bearing.x;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y);
			GLfloat w = ch.Size.x;
			GLfloat h = ch.Size.y;

			/*xpos = 1.2f * i;
			ypos = 1.0f;
			w = 1.0f;
			h = 1.0f;*/
			
			RenderedCharacter rChar = renderedCharacters[i];
			//rChar.vertices[0] = Vertex(, , , ;
			rChar.vertices[0] = Vertex(glm::vec3(xpos,     ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rChar.vertices[1] = Vertex(glm::vec3(xpos,     ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rChar.vertices[2] = Vertex(glm::vec3(xpos + w, ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rChar.vertices[3] = Vertex(glm::vec3(xpos,     ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rChar.vertices[4] = Vertex(glm::vec3(xpos + w, ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rChar.vertices[5] = Vertex(glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
/*
			rChar.vertices[1] = { glm::vec3(xpos,     ypos,     0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
			rChar.vertices[2] = { glm::vec3(xpos + w, ypos,     0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
			rChar.vertices[3] = { glm::vec3(xpos,     ypos + h, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
			rChar.vertices[4] = { glm::vec3(xpos + w, ypos,     0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
			rChar.vertices[5] = { glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
*/
			rChar.texture = ch.TextureID;
			renderedCharacters[i] = rChar;
			// Update content of VBO memory

			for(int k = 0; k < 6; k++) {
				vertices.push_back(rChar.vertices[k]);
			}
			
			//glBufferSubData(GL_ARRAY_BUFFER, i * 6, sizeof(Vertex) * 6, rChar.vertices);

			// Now advance cursors for next glyph (note that advance is numberof 1 / 64 pixels)
			x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)

			if (size.width < xpos + w) {
				size.width = xpos + w;
			}
			if (size.height < ypos + h) {
				size.height = ypos + h;
			}
		}
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
		glBindVertexArray(0);
	}
	this->text = text;
}

std::string Text::getText()
{
	return text;
}

void Text::setup() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
