#pragma once

#include "Core/Scene/drawable.h"
#include "Core/Renderer/DeferredRenderer/Interface/deferred_sceneobject.h"
/**
 * @brief Interface for drawables in FlatRenderer
 * 
 */
class DeferredDrawable : public DeferredSceneObject {
public:
    /**
     * @brief Construct a new Flat Drawable from @ref Drawable
     * Calls setup function
     * @param drawable 
     */
    DeferredDrawable(Drawable * drawable);
    /**
     * @brief Updates internal data
     * Will be called by mapped class when @see Drawable::isModified is true
     */
    virtual void update() override;

    /**
     * @brief Draws data to screen with features (outline, xray,..)
     * 
     */
    virtual void draw() override;

    /**
     * @brief Draws minimum data (textures, mesh,..)
     *  
     *  @param shader Provide a custom shader
     */
    virtual void drawRaw(Shader shader) override;

    /**
     * @brief Draw mesh data only
     * 
     * Plain vertices
     * 
     * @param shader 
     */
    virtual void drawMesh(Shader shader) override;

    /**
     * @brief Frees allocated memeory
     * 
     */
    virtual void dispose() override;
protected:
    /**
     * @brief Initialize varibales
     * 
     */
    virtual void setup() override;
};