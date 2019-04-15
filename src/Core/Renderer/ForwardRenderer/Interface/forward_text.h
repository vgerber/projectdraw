#pragma once

#include "Core/Scene/Text/text.h"
#include "Core/Renderer/ForwardRenderer/Interface/forward_drawable.h"
#include "Core/Resource/resourcemanager.h"


class ForwardText : public ForwardDrawable {
public:
    ForwardText(Text * text);

    virtual void update() override;

    virtual void draw() override;

    virtual void dispose() override;

protected:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;
    std::vector<RenderedCharacter> renderedCharacters;

     virtual void setup() override;
};