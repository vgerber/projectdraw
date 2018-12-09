#pragma once


#include "Core/Scene/drawable.h"

#include "physicsobject.h"
#include "../Collision/Shapes/cshape.h"

typedef  unsigned int (uint);

enum RigidType
{
	STATIC,
	KINEMAITC,
	DYNAMIC
};

class RigidBody : public PhyscisObject {
public:
    bool visibleAABB = false;

    RigidBody();
    RigidBody(collision::CollisionShape shape, float mass, RigidType type);

	void update() override;

	void linkDrawable(Drawable &drawable);

	void refreshBody();
	void refreshDrawable();

    void drawAABB(Shader shader);

	void dispose();

	//Rigidbody manipulators

	void setDamping(float linear, float angular);
	void setRestitution(float restitution);

	//instant
	void applyImpulse(glm::vec3 impulse, glm::vec3 relativePosition = glm::vec3(0.0f));
	//time based 
	void applyForce(glm::vec3 force, glm::vec3 relativePosition = glm::vec3(0.0f));

	void setLinearFactor(glm::vec3 factor);

	void setAngularFactor(glm::vec3 factor);

	float getLinearDamping();
	float getAngularDamping();

	float getRestitution();
	
	glm::vec3 getLinearFactor();
	glm::vec3 getAngularFactor();

	glm::vec3 getLinearVelocity();
	glm::vec3 getAngularVelocity();


	RigidType getRigidType();

	btRigidBody* getBody();
	Drawable* getDrawable();
private:
	glm::vec3 oldScale;
	RigidType rType;
    collision::CollisionShape *shape = nullptr;
    btRigidBody* rigidBody = nullptr;
	Drawable* drawable = nullptr;

    GLuint aabbVAO, aabbVBO;

	void scaleShape(glm::vec3 scale);
};