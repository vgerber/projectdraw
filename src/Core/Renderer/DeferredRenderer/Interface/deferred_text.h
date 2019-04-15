#pragma once

#include "Core/Scene/Text/text.h"
#include "Core/Renderer/DeferredRenderer/Interface/deferred_drawable.h"
#include "Core/Resource/resourcemanager.h"


class DeferredText : public DeferredDrawable {
public:
    DeferredText(Text * text);

    virtual void update() override;

    virtual void draw() override;

    virtual void drawRaw(Shader shader) override;

    virtual void dispose() override;

protected:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;
    std::vector<RenderedCharacter> renderedCharacters;

     virtual void setup() override;
};