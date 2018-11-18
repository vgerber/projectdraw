#pragma once

#include <functional>
#include <random>

#include "../drawable.h"
#include "../Instancing/instancer.h"
#include "../Animation/animatable.h"

struct Particle {
	int cycle_count = 0;
	GLfloat timer = 2.0f;
	bool alive = false;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class ParticleGenerator : public Drawable, public Animatable {
public:
	ParticleGenerator(Drawable drawable, int particleCount);

	void draw() override;
	void update(GLfloat deltaTime) override;

	virtual void dispose();

	~ParticleGenerator();
private:
	Instancer* instancer = nullptr;
	Drawable drawableParticle;
	std::vector<Particle> particles;
	bool replay = false;
	float respawnTimer = 0.0f;
};