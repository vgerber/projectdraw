#pragma once

#include "Core/Scene/sceneobject.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Shader/shader.h"
#include "Core/Renderer/Interface/renderobject.h"


class DeferredSceneObject : public RenderObject {
public:
    /**
     * @brief Construct a new Flat Scene Object from @ref SceneObject
     * 
     * @param sceneObject 
     */
    DeferredSceneObject(SceneObject * sceneObject);

    /**
     * @brief Draw object with all features
     * 
     * e.g outline, xray,...
     * 
     */
    virtual void draw();

    /**
     * @brief Draw object minimum (like textures, mesh,..)
     * 
     * Useful if silhouette of object is needed
     * 
     * @param shader Provide a shader as rendering operation
     */
    virtual void drawRaw(Shader shader);

    /**
     * @brief Draw mesh only
     * 
     * Draws plain vertex data
     * 
     * @param shader 
     */
    virtual void drawMesh(Shader shader);

    /**
     * @brief Set the Camera properties
     * 
     * @param camera 
     */
    virtual void setCamera(const Camera &camera);
protected:
    glm::mat4 mvp = glm::mat4(1); //Model View Projection Matrix

    /**
     * @brief Sets and allocates resources
     * 
     */
    virtual void setup() override;
};