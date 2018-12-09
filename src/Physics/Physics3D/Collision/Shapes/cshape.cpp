#include "cshape.h"

using namespace collision;

CollisionShape::CollisionShape(btCollisionShape* shape) {
    this->shape = shape;
}


btCollisionShape* CollisionShape::getShape() {
    return shape;
}

void CollisionShape::dispose() {
    delete shape;
}

btCollisionShape* collision::generateCube(Size size) {
    return new btBoxShape(btVector3(size.width * .5f, size.height * .5f, size.depth * .5f));
}

btCollisionShape* collision::generateSphere(GLfloat radius) {
    return new btSphereShape(btScalar(radius));
}

btCollisionShape * collision::generateTriangleMesh(Model model)
{
	std::vector<BasicMesh> meshes = model.getMeshes();
	btTriangleMesh *triangleMesh = new btTriangleMesh();

	for (BasicMesh mesh : meshes) {
		std::vector<Vertex> vertices = mesh.getVertices();
		std::vector<GLuint> indices = mesh.getIndices();

		for (size_t i = 0; i < indices.size(); i += 3) {
			triangleMesh->addTriangle(
				toBtVec3(vertices[indices[i]].Position),
				toBtVec3(vertices[indices[i+1]].Position),
				toBtVec3(vertices[indices[i+2]].Position),
				true
			);
		}
	}

	return new btBvhTriangleMeshShape(triangleMesh, true);
}

btCollisionShape * collision::generateHeightField(std::vector<float> data, int width, int length)
{
	float min, max;
	bool isFirst = true;
	for (auto d : data) {
		if (isFirst) {
			min = d;
			max = d;
			isFirst = false;
		}
		if (d < min) {
			min = d;
		}
		if (d > max) {
			max = d;
		}
	}

	return new btHeightfieldTerrainShape(
		width,
		length,
		&data[0],
		1.0f,
		min,
		max,
		1,
		PHY_FLOAT,
		false
	);
}