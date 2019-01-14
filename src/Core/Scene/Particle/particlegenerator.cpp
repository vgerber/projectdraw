#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(Drawable & drawable, int particleCount)
{
	this->particleDrawable = &drawable;
	particles = std::vector<Particle>(particleCount);
	instancer = new Instancer(this->particleDrawable, particleCount);
}

void ParticleGenerator::draw(DrawType drawType)
{
	std::vector<glm::mat4> matrices = instancer->getModelMatrices();
	for (size_t i = 0; i < particles.size(); i++) {
		Particle part = particles[i];
		if (part.alive) {
			particleDrawable->setPosition(part.position);
			//drawableParticle.rotate(part.rotation);
			particleDrawable->scale(part.scale);

			matrices[i] = particleDrawable->getModelMatrix();
		}
		else {
			matrices[i] = glm::mat4(0);
		}
	}
	instancer->setModelMatrices(matrices);
	instancer->draw(drawType);
}

void ParticleGenerator::update(GLfloat deltaTime)
{
	int cycle_revive = std::floor(deltaTime / (5.0f / particles.size()));
	respawnTimer -= deltaTime;
	for (Particle &part : particles) {
		if (part.alive) {
			part.timer -= deltaTime;
			if (part.timer <= 0.0f) {
				part.alive = false;
				part.cycle_count++;
			}

			part.position.x += ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f) * 1.0f * deltaTime * 6.0f;
			part.position.y += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 6.f;
			part.position.z += ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f - 10.0f) * 1.0f * deltaTime * 6.0f;

			part.scale.x += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
			part.scale.y += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
			part.scale.z += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
		}
		else {
			if (cycle_revive > 0) {
				part.alive = true;
				part.timer = 5.0f;
				cycle_revive--;
				part.position = getPosition();
				part.scale = glm::vec3(1.0f, 1.0f, 1.0f);
				//respawnTimer = part.timer / particles.size();
			}
		}
	}
}

void ParticleGenerator::dispose()
{
	particleDrawable->dispose();
}

ParticleGenerator::~ParticleGenerator()
{
	delete instancer;
}
