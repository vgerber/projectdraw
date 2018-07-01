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
		std::vector<Vertex> vertices = mesh.get_vertices();
		std::vector<GLuint> indices = mesh.get_indices();

		for (int i = 0; i < indices.size(); i += 3) {
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
