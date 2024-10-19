#include "rigidbody.h"

RigidBody::RigidBody() {}

RigidBody::RigidBody(collision::CollisionShape shape, float mass,
                     RigidType type) {
  this->shape = new collision::CollisionShape(shape);
  btTransform transform;
  transform.setIdentity();

  btScalar bt_mass(mass);

  if (type == RigidType::KINEMATIC) {
    mass = 0.0f;
  }

  btVector3 localInertia(0, 0, 0);
  if (type != RigidType::STATIC) {
    this->shape->getShape()->calculateLocalInertia(bt_mass, localInertia);
  }

  btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(
      mass, myMotionState, this->shape->getShape(), localInertia);
  collisionObject = new btRigidBody(rbInfo);
  rigidBody = static_cast<btRigidBody *>(collisionObject);

  if (type == RigidType::KINEMATIC) {
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() |
                                 btCollisionObject::CF_KINEMATIC_OBJECT);
    rigidBody->setActivationState(DISABLE_DEACTIVATION);
  }
  rigidBody->setUserPointer(this);
}

void RigidBody::update() {
  CollisionObject::update();
  refreshDrawable();
}

void RigidBody::linkDrawable(Drawable &drawable) {
  this->drawable = &drawable;
  oldScale = drawable.getScale();
  scaleShape(drawable.getScale());

  refreshBody();
}

void RigidBody::refreshBody() {
  if (drawable) {
    btTransform transform;
    glm::vec3 center = drawable->getPosition();
    glm::vec3 rotation = drawable->getRotator().getRotationEuler();

    scaleShape(drawable->getScale());

    transform.setIdentity();
    transform.setOrigin(btVector3(center.x, center.y, center.z));
    transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
    rigidBody->setWorldTransform(transform);
    rigidBody->getMotionState()->setWorldTransform(transform);
  }
}

void RigidBody::refreshDrawable() {
  if (drawable) {
    btTransform transform;

    transform = rigidBody->getWorldTransform();
    glm::vec3 position =
        glm::vec3(transform.getOrigin().getX(), transform.getOrigin().getY(),
                  transform.getOrigin().getZ());
    glm::vec3 rotationEuler;
    Rotator rotator;
    btVector3 btRotationAxis = transform.getRotation().getAxis();
    btScalar btRotationAngle = transform.getRotation().getAngle();
    rotator.rotateAxis((float)btRotationAngle,
                       glm::vec3(btRotationAxis.getX(), btRotationAxis.getY(),
                                 btRotationAxis.getZ()));
    // if(rType == RigidType::STATIC)
    //	transform.setOrigin(transform.getOrigin() +
    // btVector3(drawable->getSize().width, drawable->getSize().height,
    // drawable->getSize().depth) * btVector3(0.5f, 0.5f, 0.5f)); position =
    // toVec3(transform.getOrigin());
    drawable->translate(position);
    drawable->rotate(rotator);
  }
}

void RigidBody::dispose() {
  delete rigidBody->getMotionState();
  delete rigidBody;
}

void RigidBody::setDamping(float linear, float angular) {
  rigidBody->setDamping(linear, angular);
}

void RigidBody::setRestitution(float restitution) {
  rigidBody->setRestitution(restitution);
}

void RigidBody::applyImpulse(glm::vec3 impules, glm::vec3 relativePosition) {
  rigidBody->applyImpulse(toBtVec3(impules), toBtVec3(relativePosition));
}

void RigidBody::applyForce(glm::vec3 force, glm::vec3 relativePosition) {
  rigidBody->applyForce(toBtVec3(force), toBtVec3(relativePosition));
}

void RigidBody::setLinearFactor(glm::vec3 factor) {
  rigidBody->setLinearFactor(toBtVec3(factor));
}

void RigidBody::setAngularFactor(glm::vec3 factor) {
  rigidBody->setAngularFactor(toBtVec3(factor));
}

float RigidBody::getLinearDamping() { return rigidBody->getLinearDamping(); }

float RigidBody::getAngularDamping() { return rigidBody->getAngularDamping(); }

float RigidBody::getRestitution() { return rigidBody->getRestitution(); }

glm::vec3 RigidBody::getLinearFactor() {
  return toGLMVec3(rigidBody->getLinearFactor());
}

glm::vec3 RigidBody::getAngularFactor() {
  return toGLMVec3(rigidBody->getAngularFactor());
}

glm::vec3 RigidBody::getLinearVelocity() {
  return toGLMVec3(rigidBody->getLinearVelocity());
}

glm::vec3 RigidBody::getAngularVelocity() {
  return toGLMVec3(rigidBody->getAngularVelocity());
}

RigidType RigidBody::getRigidType() { return rType; }

btCollisionObject *RigidBody::getCollisionObjectHandle() { return rigidBody; }

Drawable *RigidBody::getDrawable() { return drawable; }

Transform RigidBody::getWorldTransform() {
  btTransform bodyTransform;
  rigidBody->getMotionState()->getWorldTransform(bodyTransform);
  btQuaternion bodyRotation = bodyTransform.getRotation();
  Transform newTransform;
  newTransform.translate(toGLMVec3(bodyTransform.getOrigin()));
  newTransform.rotate(Rotator(glm::quat(bodyRotation.x(), bodyRotation.y(),
                                        bodyRotation.z(), bodyRotation.w()),
                              glm::vec3(0.0)));
  return newTransform;
}

void RigidBody::scaleShape(glm::vec3 scale) {
  if (rType == RigidType::DYNAMIC) {
    oldScale = scale / oldScale;
    shape->getShape()->setLocalScaling(
        btVector3(oldScale.x, oldScale.y, oldScale.z));
    oldScale = drawable->getScale();
  } else {
    oldScale = drawable->getScale();
    glm::vec3 scale = drawable->getScale();
    shape->getShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
  }
}

void RigidBody::transformChanged() {
  // rigidBody->clearForces();
  btTransform worldTransform = btTransform::getIdentity();
  worldTransform.setOrigin(toBtVec3(transform.getTranslation()));
  glm::quat rot = transform.getRotation().getRotation();
  btQuaternion quaternion(rot.x, rot.y, rot.z, rot.w);
  worldTransform.setRotation(quaternion);
  rigidBody->setWorldTransform(worldTransform);
  if (rigidBody->getMotionState()) {
    rigidBody->getMotionState()->setWorldTransform(worldTransform);
  }
}
