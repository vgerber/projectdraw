#pragma once

#include "Core/Renderer/FlatRenderer/Interface/flat_drawable.h"
#include "Core/Scene/Particle/2D/point_generator.h"
#include "Core/Resource/resourcemanager.h"

class FlatPointGenerator : public FlatDrawable {
public:
    /**
     * @brief Construct a new Forward PointGenerator from @ref Drawable
     * Calls setup function
     * @param drawable 
     */
    FlatPointGenerator(PointGenerator2D * pointGenerator);
    /**
     * @brief Updates internal data
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
    unsigned int particleCount = 0;
    unsigned int VAO;
    unsigned int VBOpos, VBOcolor;

    /**
     * @brief Initialize varibales
     * 
     */
    virtual void setup() override;
};