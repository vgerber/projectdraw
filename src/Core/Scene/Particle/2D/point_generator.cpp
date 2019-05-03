#include "point_generator.h"

PointGenerator2D::PointGenerator2D(unsigned int particleCount) {
    setParticleCount(particleCount);
    float shift = 1.0f / particleCount;
    for(int i = 0; i < particleCount; i++) {
        particles.position[i] = glm::vec2(sin((i * shift) * 6.0f), cos((i * shift) * 6.0f));
        particles.color[i] = glm::vec4(1.0 - (shift * i), (shift * i), 1.0, 1.0);
    }
    callUpdate();
}

void PointGenerator2D::update(float delta) {
    float speed = 0.5f;
    float lifeTime = particles.lifeTime[0] + delta;
    float shift = (1.0f / particleCount);
    for(int i = 0; i < particleCount; i++) {
        particles.velocity[i] += glm::vec2(0.1 * ((float)rand() / RAND_MAX - 0.5f), 0.0);
        particles.position[i] += particles.velocity[i] * delta; //glm::vec2(sin(((i * shift) + lifeTime * speed) * 3.0f), cos(((i * shift) + lifeTime * speed) * 3.0f));
        particles.lifeTime[i] = lifeTime;
    }
    callUpdate();
}

void PointGenerator2D::reset() {
    for(int i = 0; i < particleCount; i++) {
        particles.color[i] = glm::vec4(0.0);
        particles.position[i] = glm::vec2(0.0);
        particles.velocity[i] = glm::vec2(0.0);
        particles.lifeTime[i] = particleLifetime;
    }
    callUpdate();
}

void PointGenerator2D::callUpdate() {
    if(particleCount != particles.velocity.size()) {
        particles.velocity.resize(particleCount);
        particles.position.resize(particleCount);
        particles.color.resize(particleCount);
        particles.lifeTime.resize(particleCount);
    }
    particlesPerMillisecond = particleCount / particleLifetime;
    ParticleGenerator::callUpdate();
}

PointParticles2D PointGenerator2D::getParticles() {
    return particles;
}