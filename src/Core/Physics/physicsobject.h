#pragma once

#include <btBulletDynamicsCommon.h>

#include "../Scene/sceneobject.h"
#include "CollisionShapes/cshape.h"
#include "../Scene/drawable.h"

#include <map>

typedef  unsigned int (uint);


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

	btRigidBody* getBody();
	Drawable* getDrawable();
private:
    collision::CollisionShape *shape = nullptr;
    btRigidBody* rigidBody = nullptr;
	Drawable *drawable = nullptr;

    GLuint aabbVAO, aabbVBO;
};




class PhysicsObject {
public:	
    uint addRigidBody(RigidBody rigidBody);
    RigidBody getRigidBody(uint id);
    std::map<uint, class RigidBody> getRigidBodys();

private:
    uint id_counter = 0;
    std::map<uint, class RigidBody> rigidBodys;
};