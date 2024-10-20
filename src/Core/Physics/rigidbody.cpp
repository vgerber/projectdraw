#include "rigidbody.h"

RigidBody::RigidBody() {
    rigidBody = nullptr;
    shape = nullptr;
}

RigidBody::RigidBody(collision::CollisionShape shape, glm::vec3 center, glm::vec3 rotation, GLfloat mass, bool isKinematic) {
	this->shape = new collision::CollisionShape(shape);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(center.x , center.y, center.z));
	transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
	
	btScalar bt_mass(mass);

	bool isDynamic = (bt_mass != 0.0f) || isKinematic;

	if (isDynamic) {
		if (isKinematic && bt_mass == 0.0f) {
			rType = RigidType::KINEMAITC;
		}
		else {
			rType = RigidType::DYNAMIC;
		}
	}
	else {
		rType = RigidType::STATIC;
	}

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) {
		this->shape->getShape()->calculateLocalInertia(bt_mass, localInertia);
	}

	btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, this->shape->getShape(), localInertia);
	rigidBody = new btRigidBody(rbInfo);	
	
	glGenVertexArrays(1, &aabbVAO);
	glGenBuffers(1, &aabbVBO);		

}

void RigidBody::setDrawable(Drawable &drawable)
{
	this->drawable = &drawable;
	oldScale = drawable.getScale();

	scaleShape(drawable.getScale());


	btTransform transform = rigidBody->getWorldTransform();

	transform.setOrigin(transform.getOrigin() - btVector3(drawable.getSize().width, drawable.getSize().height, drawable.getSize().depth) * btVector3(0.5f, 0.5f, 0.5f));
	rigidBody->setWorldTransform(transform);
}

void RigidBody::syncBody()
{
	if (drawable) {
		btTransform transform;
		glm::vec3 center = drawable->getPositionCenter();
		glm::vec3 rotation = drawable->getRotation();

		scaleShape(drawable->getScale());

		transform.setIdentity();
		transform.setOrigin(btVector3(center.x, center.y, center.z));
		transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
		rigidBody->setWorldTransform(transform);
		rigidBody->getMotionState()->setWorldTransform(transform);
		
	}
}

void RigidBody::syncDrawable()
{
	if (drawable) {
		btTransform transform;
		drawable->setCenter(glm::vec3(0.5f, 0.5f, 0.5f));
		
		transform = rigidBody->getWorldTransform();
		if (drawable->getScale().x > 1.0) {
			glm::vec3 position = toVec3(transform.getOrigin());
		}
		//if(rType == RigidType::STATIC)
		//	transform.setOrigin(transform.getOrigin() + btVector3(drawable->getSize().width, drawable->getSize().height, drawable->getSize().depth) * btVector3(0.5f, 0.5f, 0.5f));
		//position = toVec3(transform.getOrigin());
		drawable->transform(transform);
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
		shape->getShape()->setLocalScaling(toBtVec3(oldScale));
		oldScale = drawable->getScale();
	}
	else {
		oldScale = drawable->getScale();
		shape->getShape()->setLocalScaling(toBtVec3(drawable->getScale()));
	}
}
