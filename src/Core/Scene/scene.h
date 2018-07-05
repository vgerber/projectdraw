#pragma once

#include "../core.h"
#include "Camera/camera.h"
#include "drawable.h"
#include "Light/lights.h"
#include "../Physics/physics.h"
#include "../Util/vehicle.h"
#include "Particle/particlegenerator.h"

#include <map>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();

	void addDrawable(Drawable &drawable);
	void addRigidBody(RigidBody &rigidBody);
	void addVehicle(Vehicle &vehicle);
	void addParticleGenerator(ParticleGenerator &pg);

	void addPlight(PointLight &plight);
	void addSLight(SpotLight &sLight);

	void setCamera(Camera &camera);
	void setDlight(DirectionalLight &dlight);

	btDiscreteDynamicsWorld* getPhysicsWorld();

	virtual void draw(GLfloat delta);
	virtual void updatePhysics(GLfloat delta);

	void dispose();
private:
	//Physics
	btDiscreteDynamicsWorld *dynamicsWorld					= nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher *dispatcher						= nullptr;
	btBroadphaseInterface *overlappingPairCache				= nullptr;
	btSequentialImpulseConstraintSolver *solver				= nullptr;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;


	std::vector<RigidBody*> rigidBodys;
	std::vector<Vehicle*> vehicles;
	//Meta

	int width = 100, height = 100;

	GLuint gBufferFBO;
	GLuint rboGDepth;
	GLuint gBufferPosition, gBufferNormal, gBufferAlbedo, gBufferUseLight;
	GLuint uboMatrices;
	GLuint screenRectVBO;
	GLuint screenRectVAO;

	GLuint screenShadowVBO;
	GLuint screenShadowVAO;

	Camera* camera = nullptr;
	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	//particles
	std::vector<ParticleGenerator*> particleGenerators;

	void setup(int width, int height);
};


