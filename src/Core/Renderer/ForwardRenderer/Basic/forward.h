#pragma once

#include <iostream>

#include "Core/core.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Camera/scenecamera.h"
#include "Core/Scene/drawable.h"
#include "Core/Scene/Light/lights.h"
#include "Core/Scene/Particle/particlegenerator.h"
#include "Core/Scene/Animation/animatable.h"

#include "Core/Texture/Filter/HDR/hdr.h"
#include "Core/Texture/Filter/SSAO/ssao.h"


#include "Core/Renderer/arenderer.h"

#include "Core/Renderer/ForwardRenderer/Interface/forward_sceneobject.h"
#include "Core/Renderer/ForwardRenderer/Interface/forward_mesh.h"
#include "Core/Renderer/ForwardRenderer/Interface/forward_text.h"
#include "Core/Renderer/ForwardRenderer/Interface/Particle/2D/fpoint_generator.h"
#include "Core/Renderer/ForwardRenderer/Interface/Particle/3D/fpoint_generator.h"
#include "Core/Renderer/ForwardRenderer/Interface/Particle/2D/fbillboard_generator.h"
#include "Core/Renderer/ForwardRenderer/Interface/Particle/3D/fbillboard_generator.h"

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
    unsigned int rendererTexture;
	unsigned int rendererFBO;
	unsigned int rendererRBO;

    unsigned int depthInfoFBO;
    unsigned int depthInfoRBO;
    unsigned int depthInfoPositionTexture;
    unsigned int depthInfoNormalTexture;
    
	unsigned int uboMatrices;

    unsigned int rectVAO, rectVBO;

    std::vector<ForwardSceneObject*> sceneObjects;
    DirectionalLight *directionalLight = nullptr;

    Shader shaderMesh;

    HDR hdrFilter;
    SSAOFilter ssaoFilter;

    virtual void setup();

private:
    ForwardSceneObject * generateForwardObject(SceneObject * sceneObject);
};