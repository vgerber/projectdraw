#pragma once

#include "Core/Scene/sceneobject.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Renderer/Interface/renderobject.h"

class FlatSceneObject : public RenderObject {
public:
    /**
     * @brief Construct a new Flat Scene Object from @ref SceneObject
     * 
     * @param sceneObject 
     */
    FlatSceneObject(SceneObject * sceneObject);

    /**
     * @brief Draw object
     * 
     */
    virtual void draw();

    virtual void setCamera(const Camera &camera);
protected:
    glm::mat4 mvp = glm::mat4(1); //Model View Projection Matrix

    /**
     * @brief Sets and allocates resources
     * 
     */
    virtual void setup() override;
};