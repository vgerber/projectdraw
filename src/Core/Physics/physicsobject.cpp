#include "physicsobject.h"

RigidBody::RigidBody() {
    rigidBody = nullptr;
    shape = nullptr;
}

RigidBody::RigidBody(collision::CollisionShape shape, glm::vec3 center, glm::vec3 rotation, GLfloat mass) {
	this->shape = new collision::CollisionShape(shape);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(center.x, center.y, center.z));
	transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
	
	btScalar bt_mass(mass);

	bool isDynamic = true; // (bt_mass != 0.0f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) {
		this->shape->get_shape()->calculateLocalInertia(bt_mass, localInertia);
	}

	btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, this->shape->get_shape(), localInertia);
	rigidBody = new btRigidBody(rbInfo);	
	
	glGenVertexArrays(1, &aabbVAO);
	glGenBuffers(1, &aabbVBO);		
}

void RigidBody::setDrawable(Drawable &drawable)
{
	this->drawable = &drawable;
}

void RigidBody::syncBody()
{
	btTransform transform;
	glm::vec3 center = drawable->getPositionCenter();
	glm::vec3 rotation = drawable->getRotation();


	transform.setIdentity();
	transform.setOrigin(btVector3(center.x, center.y, center.z));
	transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
	rigidBody->setWorldTransform(transform);	
	rigidBody->getMotionState()->setWorldTransform(transform);
	
	/*transform.setFromOpenGLMatrix(glm::value_ptr(drawable->getModelMatrix()));
	transform.setOrigin(btVector3(center.x, center.y, center.z));
	rigidBody->setWorldTransform(transform);
	*/

}

void RigidBody::syncDrawable()
{
	btTransform transform; // = rigidBody->getWorldTransform();
	drawable->setCenter(glm::vec3(0.5f, 0.5f, 0.5f));
	if (false && rigidBody && rigidBody->getMotionState()) {
		rigidBody->getMotionState()->getWorldTransform(transform);
	}
	else {
		transform = rigidBody->getWorldTransform();
	}
	//std::cout << transform.getOrigin().getY() << std::endl;
	drawable->transform(transform);
}

void RigidBody::drawAABB(Shader shader) {
	btVector3 min, max;
	rigidBody->getAabb(min, max);

	Size size;
	size.x = min.getX();
	size.y = min.getY();
	size.z = min.getZ();
	size.width = max.getX() - size.x;
	size.height = max.getY() - size.y;
	size.depth = max.getZ() - size.z;

	GLfloat vertices_box[] = {
		size.x,				 size.y,			   size.z,
		size.x + size.width, size.y,			   size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x,				 size.y,			   size.z + size.depth,
		size.x + size.width, size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z + size.depth,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y + size.height, size.z,
	};

	glBindVertexArray(aabbVAO);
	glBindBuffer(GL_ARRAY_BUFFER, aabbVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), &vertices_box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);


}

void RigidBody::dispose()
{
	delete rigidBody->getMotionState();
	delete rigidBody;
}

btRigidBody* RigidBody::getBody() {
    return rigidBody;
}

Drawable * RigidBody::getDrawable()
{
	return drawable;
}



uint PhysicsObject::addRigidBody(RigidBody rigidBody) {
    rigidBodys[id_counter] = rigidBody;
    return id_counter++;
}

RigidBody PhysicsObject::getRigidBody(uint id) {
    return rigidBodys[id];
}

std::map<uint, class RigidBody> PhysicsObject::getRigidBodys() {
    return rigidBodys;
}