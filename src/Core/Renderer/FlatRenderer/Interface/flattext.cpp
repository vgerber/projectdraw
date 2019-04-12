#include "flattext.h"

FlatText::FlatText(Text * text) : FlatDrawable(static_cast<Drawable*>(text)) {
    setup();
}

void FlatText::update() {
    Text * text = static_cast<Text*>(getLinkedObject());
    Font font = text->getFont();

    renderedCharacters.resize(text->getText().size());
    vertices.clear();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat x = 0.0f, y = 0.0f; // position.x, y = position.y;
    for (int i = 0; i < text->getText().size(); i++) {
        Character ch = font.characters[text->getText()[i]];

        GLfloat xpos = x + ch.Bearing.x;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y);
        GLfloat w = ch.Size.x;
        GLfloat h = ch.Size.y;
        
        RenderedCharacter rChar = renderedCharacters[i];
        rChar.vertices[0] = Vertex(glm::vec3(xpos,     ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rChar.vertices[1] = Vertex(glm::vec3(xpos,     ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rChar.vertices[2] = Vertex(glm::vec3(xpos + w, ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rChar.vertices[3] = Vertex(glm::vec3(xpos,     ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rChar.vertices[4] = Vertex(glm::vec3(xpos + w, ypos    , 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rChar.vertices[5] = Vertex(glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        rChar.texture = ch.TextureID;
        renderedCharacters[i] = rChar;
        // Update content of VBO memory

        for(int k = 0; k < 6; k++) {
            vertices.push_back(rChar.vertices[k]);
        }
        
        //glBufferSubData(GL_ARRAY_BUFFER, i * 6, sizeof(Vertex) * 6, rChar.vertices);

        // Now advance cursors for next glyph (note that advance is numberof 1 / 64 pixels)
        x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)

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

void FlatText::draw() {
    Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Flat::Mesh);

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

    glUniform1i(glGetUniformLocation(shader.getId(), "alphaTexture"),    0);
    glUniform1i(glGetUniformLocation(shader.getId(), "diffuseTexture"),  1);
    glUniform1i(glGetUniformLocation(shader.getId(), "specularTexture"), 2);

    glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"), 1);
    glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), 0);

    Text * text = static_cast<Text*>(getLinkedObject());

    //disable face culling for text
    glDisable(GL_CULL_FACE);

    for(int i = 0; i < text->getText().size(); i++) {
        //set texture
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedCharacters[i].texture);

        //bind vertices and draw a character
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, i * 6, 6);
        glBindVertexArray(0);


    }

    glEnable(GL_CULL_FACE);
}

void FlatText::dispose() {

}

void FlatText::setup() {
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}