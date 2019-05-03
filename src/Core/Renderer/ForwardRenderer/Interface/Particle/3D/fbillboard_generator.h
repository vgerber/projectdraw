#pragma once

#include "Core/Renderer/ForwardRenderer/Interface/forward_drawable.h"
#include "Core/Scene/Particle/3D/billboard_generator.h"
#include "Core/Resource/resourcemanager.h"
#include "Core/Util/Debug/debug.hpp"

struct SortBillboard3D {
    glm::vec3 cameraPos;

	SortBillboard3D(glm::vec3 camerPos) : cameraPos(cameraPos) {}

	inline bool operator() (glm::vec3 p1, glm::vec3 p2) {
        //just wrong...
		return glm::length2(p1 - cameraPos) > glm::length(p2 - cameraPos);
	}
};

class ForwardBillboardGenerator3D : public ForwardDrawable {
public:    
    ForwardBillboardGenerator3D(BillboardGenerator3D * billboardGenerator);
    
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