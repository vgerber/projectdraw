#include "rigidbody.h"

RigidBody::RigidBody() {
    rigidBody = nullptr;
    shape = nullptr;
}

RigidBody::RigidBody(collision::CollisionShape shape, float mass, RigidType type) {
	this->shape = new collision::CollisionShape(shape);
	btTransform transform;
	transform.setIdentity();
	
	btScalar bt_mass(mass);


	if (type == RigidType::KINEMAITC) {
		mass = 0.0f;
	}

	btVector3 localInertia(0, 0, 0);
	if (type != RigidType::STATIC) {
		this->shape->getShape()->calculateLocalInertia(bt_mass, localInertia);
	}

	btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, this->shape->getShape(), localInertia);
	rigidBody = new btRigidBody(rbInfo);	
	
	glGenVertexArrays(1, &aabbVAO);
	glGenBuffers(1, &aabbVBO);		

}

void RigidBody::linkDrawable(Drawable &drawable)
{
	this->drawable = &drawable;
	oldScale = drawable.getScale();

	scaleShape(drawable.getScale());


	btTransform transform = rigidBody->getWorldTransform();

	transform.setOrigin(transform.getOrigin() - btVector3(drawable.getSize().width, drawable.getSize().height, drawable.getSize().depth) * btVector3(0.5f, 0.5f, 0.5f));
	rigidBody->setWorldTransform(transform);
}

void RigidBody::refreshBody()
{
	if (drawable) {
		btTransform transform;
		glm::vec3 center = drawable->getPositionCenter();
		glm::vec3 rotation = drawable->getRotator().getRotationEuler();

		scaleShape(drawable->getScale());

		transform.setIdentity();
		transform.setOrigin(btVector3(center.x, center.y, center.z));
		transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
		rigidBody->setWorldTransform(transform);
		rigidBody->getMotionState()->setWorldTransform(transform);
		
	}
}

void RigidBody::refreshDrawable()
{
	if (drawable) {
		btTransform transform;
		drawable->setCenter(glm::vec3(0.5f, 0.5f, 0.5f));
		
		transform = rigidBody->getWorldTransform();
		glm::vec3 position = glm::vec3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
		glm::vec3 rotationEuler;
		Rotator rotator;
		btVector3 btRotationAxis = transform.getRotation().getAxis();
		btScalar btRotationAngle = transform.getRotation().getAngle();
		rotator.rotateAxis((float)btRotationAngle, glm::vec3(btRotationAxis.getX(), btRotationAxis.getY(), btRotationAxis.getZ()));
		//if(rType == RigidType::STATIC)
		//	transform.setOrigin(transform.getOrigin() + btVector3(drawable->getSize().width, drawable->getSize().height, drawable->getSize().depth) * btVector3(0.5f, 0.5f, 0.5f));
		//position = toVec3(transform.getOrigin());
		drawable->setPosition(position);
		drawable->rotate(rotator);
	}
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

RigidType RigidBody::getRigidType()
{
	return rType;
}

btRigidBody* RigidBody::getBody() {
    return rigidBody;
}

Drawable * RigidBody::getDrawable()
{
	return drawable;
}

void RigidBody::scaleShape(glm::vec3 scale)
{
	if (rType == RigidType::DYNAMIC) {
		oldScale = scale / oldScale;
		shape->getShape()->setLocalScaling(btVector3(oldScale.x, oldScale.y, oldScale.z));
		oldScale = drawable->getScale();
	}
	else {
		oldScale = drawable->getScale();
		glm::vec3 scale = drawable->getScale();
		shape->getShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}
}
