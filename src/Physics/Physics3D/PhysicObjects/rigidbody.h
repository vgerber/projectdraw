#pragma once

#include <btBulletDynamicsCommon.h>
#include "Core/Scene/drawable.h"

#include "../Collision/Shapes/cshape.h"

typedef  unsigned int (uint);

enum RigidType
{
	STATIC,
	KINEMAITC,
	DYNAMIC
};

class RigidBody {
public:
    bool visibleAABB = false;

    RigidBody();
    RigidBody(collision::CollisionShape shape, float mass, RigidType type);


	void linkDrawable(Drawable &drawable);

	void refreshBody();
	void refreshDrawable();

    void drawAABB(Shader shader);

	void dispose();

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