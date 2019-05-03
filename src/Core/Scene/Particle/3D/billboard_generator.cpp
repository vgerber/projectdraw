#include "billboard_generator.h"

BillboardGenerator3D::BillboardGenerator3D(unsigned int particleCount) : PointGenerator3D(particleCount) {
    reset();
}

void BillboardGenerator3D::update(float delta) {
    bool hasRespawned = false;
    int activateParticles = delta * particlesPerMillisecond;
    for(int i = 0; i < particleCount; i++) {
        if(particles.lifeTime[i] < particleLifetime) {
            particles.velocity[i] += glm::vec3(0.0, 0.0, 0.81) * delta;
            particles.position[i] += particles.velocity[i] * delta;
            particles.lifeTime[i] += delta;
        }
        else {
            particles.position[i] = glm::vec3(0.0);
            if(activateParticles) {
                particles.velocity[i] = glm::vec3(
                    ((float)rand() / RAND_MAX - 0.5) * 2.0 * 1.0, 
                    ((float)rand() / RAND_MAX - 0.5) * 2.0 * 1.0, 
                    0.2
                );
                particles.lifeTime[i] = 0.0f;
                hasRespawned = true; 
                activateParticles--;
            }          
        }
        
    }
    callUpdate();
}