#pragma once

#include "Core/Renderer/FlatRenderer/Interface/flat_drawable.h"
#include "Core/Scene/Particle/2D/billboard_generator.h"
#include "Core/Resource/resourcemanager.h"
#include "Core/Util/Debug/debug.hpp"

struct SortBillboard {

	SortBillboard() {}

	inline bool operator() (glm::vec2 p1, glm::vec2 p2) {
		return p1.y > p2.y;
	}
};

class FlatBillboardGenerator : public FlatDrawable {
public:    
    FlatBillboardGenerator(BillboardGenerator2D * billboardGenerator);
    
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

    virtual void setCamera(const Camera &camera) override;
protected:
    unsigned int particleCount = 0;
    unsigned int VAO;
    unsigned int VBOpos;

    glm::vec3 cameraRightWS, cameraUpWS;
    glm::mat4 view, vp, model; 

    /**
     * @brief Initialize variables
     * 
     */
    virtual void setup() override;

};