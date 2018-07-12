#pragma once

#include "../core.h"
#include "Camera/camera.h"
#include "Camera/scenecamera.h"
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
	Scene(int width, int height);
	~Scene();

	void addDrawable(Drawable &drawable);
	void addRigidBody(RigidBody &rigidBody);
	void addVehicle(Vehicle &vehicle);
	void addParticleGenerator(ParticleGenerator &pg);

	void addPlight(PointLight &plight);
	void addSLight(SpotLight &sLight);


	void removeDrawable(Drawable &drawable);
	void removeRigidBody(RigidBody &rigidBody);
	void removeVehicle(Vehicle &vehicle);
	void removeParticleGenerator(ParticleGenerator &particleGenerator);

	void removeCamera(Camera &camera);
	void enableCamera(Camera &camera, bool enable);


	//camera with size from -1.0 - 1.0
	void addCamera(Camera &camera, Size size);
	void setDlight(DirectionalLight &dlight);

	btDiscreteDynamicsWorld* getPhysicsWorld();

	virtual void draw(GLfloat delta);
	virtual void updatePhysics(GLfloat delta);

	void dispose();

	void reload(int width, int height);

private:
	int width = 100, height = 100;

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


	Shader shader_basic;
	Shader shader_light;
	Shader shader_normals;
	Shader shader_geometry;

	

	GLuint gBufferFBO;
	GLuint rboGDepth;
	GLuint gBufferPosition, gBufferNormal, gBufferAlbedo, gBufferUseLight;
	GLuint uboMatrices;
	GLuint screenRectVBO;
	GLuint screenRectVAO;

	GLuint screenShadowVBO;
	GLuint screenShadowVAO;

	std::vector<SceneCamera> cameras;
	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	//particles
	std::vector<ParticleGenerator*> particleGenerators;

	void setup();


};


