#pragma once

#include <btBulletDynamicsCommon.h>

#include "../Scene/sceneobject.h"
#include "CollisionShapes/cshape.h"
#include "../Scene/drawable.h"

#include <map>

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
    RigidBody(collision::CollisionShape shape, glm::vec3 center, glm::vec3 rotion, GLfloat mass, bool isKinematic = false);
	void setDrawable(Drawable &drawable);

	void syncBody();
	void syncDrawable();

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
	Drawable *drawable = nullptr;

    GLuint aabbVAO, aabbVBO;

	void scaleShape(glm::vec3 scale);
};