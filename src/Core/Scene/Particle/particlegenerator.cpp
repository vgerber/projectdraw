#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(Drawable drawable, int particleCount)
{
	this->drawableParticle = drawable;
	particles = std::vector<Particle>(particleCount);
}

void ParticleGenerator::draw(GLfloat deltaTime, Shader shader)
{
	for (Particle part : particles) {
		if (part.alive) {
			drawableParticle.setPosition(part.position);
			drawableParticle.rotate(part.rotation);
			drawableParticle.scale(part.scale);

			drawableParticle.draw();
		}
	}
}

void ParticleGenerator::update(GLfloat deltaTime)
{
	bool cycle_revive = false;

	for (Particle &part : particles) {
		if (part.alive) {
			part.timer -= deltaTime;
			if (part.timer <= 0.0f) {
				part.alive = false;
				part.cycle_count++;
			}

			part.position.x += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 3.0f;
			part.position.y += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 6.f;
			part.position.z += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 3.0f;

			part.scale.x += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
			part.scale.y += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
			part.scale.z += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * part.timer * deltaTime * 0.5f;
		}
		else {
			if (!cycle_revive) {
				part.alive = true;
				part.timer = 5.0f;
				cycle_revive = true;
				part.position = position;
				part.scale = glm::vec3(1.0f, 1.0f, 1.0f);
			}
		}
	}
}
