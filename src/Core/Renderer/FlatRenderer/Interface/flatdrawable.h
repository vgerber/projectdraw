#pragma once

#include "Core/Scene/drawable.h"
#include "Core/Renderer/FlatRenderer/Interface/flatsceneobject.h"
/**
 * @brief Interface for drawables in FlatRenderer
 * 
 */
class FlatDrawable : public FlatSceneObject {
public:
    /**
     * @brief Construct a new Flat Drawable from @ref Drawable
     * Calls setup function
     * @param drawable 
     */
    FlatDrawable(Drawable * drawable);
    /**
     * @brief Updates internal data
     * Will be called by mapped class when @see Drawable::isModified is true
     */
    virtual void update() override;

    /**
     * @brief Draws data to screen
     * 
     */
    virtual void draw() override;

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