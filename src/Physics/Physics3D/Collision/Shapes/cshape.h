#pragma once

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "Core/Mesh/basicmesh.h"

#include "../../Util/conversion.h"


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

	btCollisionShape* generateSphere(float radius);

	btCollisionShape* generateTriangleMesh(Mesh mesh);

	btCollisionShape* generateHeightField(std::vector<float> &data, int width, int length);
}