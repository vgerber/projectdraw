#pragma once

#include "../core.h"
#include "Camera/camera.h"
#include "drawable.h"
#include "Light/lights.h"
#include "../Physics/physics.h"

#include <map>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();

	void addDrawable(Drawable &drawable);
	void addRigidBody(RigidBody &rigidBody);

	void addPlight(PointLight &plight);

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
	//Meta

	int width = 100, height = 100;

	GLuint gBufferFBO;
	GLuint rboGDepth;
	GLuint gBufferPosition, gBufferNormal, gBufferAlbedo, gBufferUseLight;
	GLuint uboMatrices;
	Camera* camera = nullptr;
	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;

	void setup(int width, int height);
};


