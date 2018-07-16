#pragma once

#include "../../Model/box.h"
#include "../../Model/model.h"

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>




namespace collision {
	class CollisionShape {
	public:
		CollisionShape(btCollisionShape *shape);
		btCollisionShape* getShape();


		void dispose();
	protected:
		btCollisionShape * shape;
	};

	btCollisionShape* generateCube(Size size);

	btCollisionShape* generateSphere(GLfloat radius);

	btCollisionShape* generateTriangleMesh(Model model);

	btCollisionShape* generateHeightField(std::vector<float> data, int width, int length);
}