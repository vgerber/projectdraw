#pragma once

#include <random>

#include "Core/Scene/Particle/particlegenerator.h"



struct PointParticles2D {
    std::vector<glm::vec2> velocity;
    std::vector<glm::vec2> position;
    std::vector<glm::vec4> color;
    std::vector<float> lifeTime;
};

class PointGenerator2D : public ParticleGenerator {
public:
    PointGenerator2D(unsigned int particleCount);

    virtual void update(float delta) override;

    virtual void reset() override;

    virtual void callUpdate() override;

    PointParticles2D getParticles();

protected:
    PointParticles2D particles;
};