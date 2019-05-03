#pragma once

#include "Core/Resource/resourcemanager.h"
#include "Core/Renderer/ForwardRenderer/Interface/forward_drawable.h"
#include "Core/Scene/Particle/3D/point_generator.h"


class ForwardPointGenerator3D : public ForwardDrawable {
public:
    /**
     * @brief Construct a new Forward PointGenerator from @ref Drawable
     * Calls setup function
     * @param drawable 
     */
    ForwardPointGenerator3D(PointGenerator3D * pointGenerator);
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
     * @brief Draw data to depth info buffer
     * 
     */
    virtual void drawDepthInfo() override;

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