#pragma once

#include "Core/Scene/sceneobject.h"


/**
 * @brief Base wrapper for @ref SceneObject for renderer
 * 
 */
class RenderObject {
public:
    /**
     * @brief Construct a new Render Object and link it to an object
     * 
     * @param sceneObject 
     */
    RenderObject(SceneObject * sceneObject);

    /**
     * @brief Updates internal data
     * 
     * Does not update tree structure
     */
    virtual void update();

    /**
     * @brief Frees allocated resources
     * 
     */
    virtual void dispose();

    /**
     * @brief Get the Linked Object
     * 
     * @return SceneObject* 
     */
    SceneObject * getLinkedObject() const;

    virtual bool operator==(const RenderObject& a) const;

    std::vector<RenderObject*> children;
protected:
    SceneObject * sceneObject;    

    /**
     * @brief Setup internal data
     * 
     */
    virtual void setup();
};