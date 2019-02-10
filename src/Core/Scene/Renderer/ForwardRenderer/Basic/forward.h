#pragma once

#include <iostream>

#include "Core/core.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Camera/scenecamera.h"
#include "Core/Scene/drawable.h"
#include "Core/Scene/Light/lights.h"
#include "Core/Scene/Particle/particlegenerator.h"
#include "Core/Scene/Instancing/instancer.h"
#include "Core/Scene/Animation/animatable.h"
#include "Core/Scene/Text/text.h"
#include "Core/Texture/Filter/HDR/hdr.h"

#include "Core/Scene/Renderer/arenderer.h"

class ForwardRenderer : public AbstractRenderer {
public:
    ForwardRenderer(int width, int height, Camera &camera);

    /*
    * Resize internal storage
    */
    virtual void resize(int width, int height) override;

    /*
    * Clear render buffers
    */
    virtual void clearScreen() override;

    /*
    * Render meshes
    */
    virtual void render() override;

    /*
    * Add object to the render qeue
    */
	virtual void addSceneObject(SceneObject &sceneObject) override;

    /*
    * Remove object from render qeue
    */
	virtual void removeSceneObject(SceneObject &sceneObject) override;

    /*
    * Get texture with render() results
    */
    virtual GLuint getTexture() override;

    /*
    * Free memory
    */
    virtual void dispose() override;

protected:
    uint rendererTexture;
    uint rendererFBO;
    uint rendererRBO;
    
    uint uboMatrices;

    std::vector<Drawable*> drawables;
    DirectionalLight *directionalLight = nullptr;

    Shader shaderMesh;

    HDR hdrFilter;

    virtual void setup();

private:
	///Draw drawable as plain geometry (without features)
	virtual void renderDrawableRaw(Drawable * drawable, Shader shader, DrawType drawType = DrawType::TRIANGLEG);
};