#include "billboard_generator.h"

BillboardGenerator2D::BillboardGenerator2D(unsigned int particleCount) : PointGenerator2D(particleCount) {
    reset();
    callUpdate();
}

void BillboardGenerator2D::update(float delta) {
    queuedParticles += delta * particlesPerMillisecond;
    for(int i = 0; i < particleCount; i++) {
        if(particles.lifeTime[i] < particleLifetime) {
            particles.velocity[i] += glm::vec2(0.0, 9.81) * delta;
            particles.position[i] += particles.velocity[i] * delta;
            particles.lifeTime[i] += delta;
        }
        else {
            particles.position[i] = glm::vec2(0.0, 0.0);
            if(queuedParticles >= 1.0f) {
                particles.velocity[i] = glm::vec2(((float)rand() / RAND_MAX - 0.5) * 2.0 * 10.0, 10.0);
                particles.lifeTime[i] = 0.0f;
                queuedParticles--;
            }          
        }        
    }
    callUpdate();
}