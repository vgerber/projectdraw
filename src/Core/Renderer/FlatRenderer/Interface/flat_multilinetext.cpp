#include "flat_multilinetext.h"

FlatMultilineText::FlatMultilineText(MultilineText * multilineText) : FlatText(static_cast<Text*>(multilineText)) {

}

void FlatMultilineText::update()  {
    MultilineText * text = static_cast<MultilineText*>(getLinkedObject());
    Font font = text->getFont();

    int letterCount = 0;
    for(auto line : text->getTextLines()) {
        letterCount += line.size();
    }

    renderedCharacters.resize(letterCount);
    vertices.clear();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int activeRenderedChar = 0;
    float x = 0.0f;
    float y = text->getSize().height - font.getVerticalBounds().x; // position.x, y = position.y;
    for(int lineIndex  = 0; lineIndex < text->getTextLines().size(); lineIndex++) {
        std::string line = text->getTextLines()[lineIndex];
        for (int i = 0; i < line.size(); i++) {
            Character ch = font.characters[line[i]];

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
            renderedCharacters[activeRenderedChar] = rChar;
            // Update content of VBO memory

            for(int k = 0; k < 6; k++) {
                vertices.push_back(rChar.vertices[k]);
            }
            
            //glBufferSubData(GL_ARRAY_BUFFER, i * 6, sizeof(Vertex) * 6, rChar.vertices);

            // Now advance cursors for next glyph (note that advance is numberof 1 / 64 pixels)
            x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)
            activeRenderedChar++;
        }
        printf("%f\n", y);
        y -= text->getLineHeight() + (font.getVerticalBounds().x - font.getVerticalBounds().y);
        x = 0.0;        
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