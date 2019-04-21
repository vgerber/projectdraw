#pragma once

#include "Core/Renderer/DeferredRenderer/Interface/deferred_drawable.h"
#include "Core/Mesh/Primitives/mesh.h"
#include "Core/Resource/resourcemanager.h"

#include "Core/Util/Debug/debug.hpp"
/**
 * @brief Interface for mesh data mapping to renderer mesh data
 * 
 * This class provides the opengl memory layout
 * suitable for the FlatRenderer 
 * 
 */
class DeferredMesh : public DeferredDrawable {
public:
    DeferredMesh(Mesh * mesh);

    virtual void update() override;

    virtual void draw() override;

    virtual void drawRaw(Shader shader) override;

    virtual void drawMesh(Shader shader) override;

    virtual void dispose() override;
protected:
    unsigned int VBO, VAO, EBO;

    virtual void setup() override;
};