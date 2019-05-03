#include "point_generator.h"

PointGenerator3D::PointGenerator3D(unsigned int particleCount) {
    setParticleCount(particleCount);
    float shift = 1.0f / particleCount;
    for(int i = 0; i < particleCount; i++) {
        particles.velocity[i] = glm::vec3(0.0);
        particles.position[i] = glm::vec3(0.0, 0.0, 0.0);
        particles.color[i] = glm::vec4(1.0, 1.0, 1.0, 1.0);
        particles.lifeTime[i] = particleLifetime;
    }
    callUpdate();
}

void PointGenerator3D::update(float delta) {
    float speed = 0.5f;
    int newParticles = particlesPerMillisecond * delta;
    for(int i = 0; i < particleCount; i++) {
        if(particles.lifeTime[i] < particleLifetime) {
            particles.position[i] += particles.velocity[i] * delta;
            particles.position[i].z = sin(particles.lifeTime[i] * 20.0) * 0.02;
            particles.lifeTime[i] += delta;
        }
        else if(newParticles) {
            particles.position[i] = glm::vec3(0.0);
            particles.velocity[i] = glm::normalize(particles.velocity[i] += glm::vec3(
                0.8 * ((float)rand() / RAND_MAX - 0.5f),
                0.8 * ((float)rand() / RAND_MAX - 0.5f),
                0.0
            ));
            particles.color[i] = glm::vec4(0.3, 0.3, 1.0, 1.0);
            particles.lifeTime[i] = 0;
            newParticles--;
        }
    }
    callUpdate();
}

void PointGenerator3D::reset() {
    for(int i = 0; i < particleCount; i++) {
        particles.color[i] = glm::vec4(0.0);
        particles.position[i] = glm::vec3(0.0);
        particles.velocity[i] = glm::vec3(0.0);
        particles.lifeTime[i] = particleLifetime;
    }
    callUpdate();
}

void PointGenerator3D::callUpdate() {
    if(particleCount != particles.velocity.size()) {
        particles.velocity.resize(particleCount);
        particles.position.resize(particleCount);
        particles.color.resize(particleCount);
        particles.lifeTime.resize(particleCount);
    }
    particlesPerMillisecond = particleCount / particleLifetime;
    ParticleGenerator::callUpdate();
}

PointParticles3D PointGenerator3D::getParticles() {
    return particles;
}