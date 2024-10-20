#include "particlegenerator.h"

void ParticleGenerator::setParticleCount(unsigned int particleCount) {
  this->particleCount = particleCount;
  callUpdate();
}

void ParticleGenerator::setLifeTime(float lifeTime) {
  this->particleLifetime = lifeTime;
  reset();
}

void ParticleGenerator::enable(bool enabled) { active = enabled; }

Size ParticleGenerator::getSize() { return size; }
