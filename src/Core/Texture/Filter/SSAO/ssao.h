#pragma once

#include <vector>
#include <random>

#include "Core/Texture/Filter/filter.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Resource/resourcemanager.h"

/**
 * @brief SSAO Filter
 * 
 * based on https://learnopengl.com/Advanced-Lighting/SSAO
 */
class SSAOFilter : public Filter {
public:
    /**
     * @brief Apply SSAO effect to texture
     *  
     * @param texture Albedo Texture
     * @param normals Normals Texture
     * @param positions Positions Texture
     */
    virtual void apply(const Camera& camera, unsigned int normals, unsigned int positions);

    /**
     * @brief Resizes filter to target size
     * 
     * All textures have to be (width x height)
     * 
     * @param width 
     * @param height 
     */
    virtual void resize(int width, int height) override;

    /**
     * @brief Get occlusion values texture (single channel)
     * 
     * @return unsigned int 
     */
    unsigned int getGLTexture();

    /**
     * @brief Clears internel buffers
     * 
     */
    virtual void clear() override;

    /**
     * @brief Free allocated memory
     * 
     */
    virtual void dispose() override;

    /**
     * @brief Initializes internals
     * 
     */
    virtual void setup() override;
private:
    unsigned int ssaoTexture, ssaoFinalTexture, noiseTexture;
    std::vector<glm::vec3> kernel, noise;

    int width, height;
    glm::vec2 noiseScale;

    unsigned int ssaoFBO;

    unsigned int rectVAO;
    unsigned int rectVBO;

    Shader shaderSSAO;
    Shader shaderSSAOBlur;
};