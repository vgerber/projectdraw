#pragma once

#include <random>

#include "Core/Scene/Particle/particlegenerator.h"

struct PointParticles3D {
    std::vector<glm::vec3> velocity;
    std::vector<glm::vec3> position;
    std::vector<glm::vec4> color;
    std::vector<float> lifeTime;
};

class PointGenerator3D : public ParticleGenerator {
public:
    PointGenerator3D(unsigned int particleCount);

    virtual void update(float delta) override;

    virtual void reset() override;

    virtual void callUpdate() override;

    PointParticles3D getParticles();

protected:
    PointParticles3D particles;
};