#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(Model &model, int particleCount)
{
	this->particleModel = &model;
	particles = std::vector<Particle>(particleCount);
	instancer = new Instancer(this->particleModel, particleCount);
}

void ParticleGenerator::draw()
{
	shader.use();
	std::vector<glm::mat4> matrices = instancer->getModelMatrices();
	for (size_t i = 0; i < particles.size(); i++) {
		Particle part = particles[i];
		if (part.alive) {
			particleModel->setPosition(part.position);
			//drawableParticle.rotate(part.rotation);
			particleModel->scale(part.scale);

			matrices[i] = particleModel->getModelMatrix();
		}
		else {
			matrices[i] = glm::mat4(0);
		}
	}
	instancer->setModelMatrices(matrices);
	instancer->draw(shader);
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
	particleModel->dispose();
}

ParticleGenerator::~ParticleGenerator()
{
	delete instancer;
}
