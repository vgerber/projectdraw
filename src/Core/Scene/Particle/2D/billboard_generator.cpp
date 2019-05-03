#include "billboard_generator.h"

BillboardGenerator2D::BillboardGenerator2D(unsigned int particleCount) : PointGenerator2D(particleCount) {
    reset();
    callUpdate();
}

void BillboardGenerator2D::update(float delta) {
    bool hasRespawned = false;
    int activateParticles = delta * particlesPerMillisecond;
    for(int i = 0; i < particleCount; i++) {
        if(particles.lifeTime[i] < particleLifetime) {
            particles.velocity[i] += glm::vec2(0.0, 0.81) * delta;
            particles.position[i] += particles.velocity[i] * delta;
            particles.lifeTime[i] += delta;
        }
        else {
            particles.position[i] = glm::vec2(0.0, 0.0);
            if(activateParticles) {
                particles.velocity[i] = glm::vec2(((float)rand() / RAND_MAX - 0.5) * 2.0 * 1.0, 0.2);
                particles.lifeTime[i] = 0.0f;
                hasRespawned = true; 
                activateParticles--;
            }          
        }        
    }
    callUpdate();
}