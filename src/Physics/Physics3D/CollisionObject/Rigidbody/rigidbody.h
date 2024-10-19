#pragma once

#include "Core/Scene/drawable.h"

#include "../../Collision/Shapes/cshape.h"
#include "../collisionobject.h"

typedef unsigned int(uint);

enum RigidType { STATIC, KINEMATIC, DYNAMIC };

class RigidBody : public CollisionObject {
public:
  bool visibleAABB = false;

  RigidBody();
  RigidBody(collision::CollisionShape shape, float mass, RigidType type);

  void update() override;

  void linkDrawable(Drawable &drawable);

  void refreshBody();
  void refreshDrawable();

  void dispose();

  // Rigidbody manipulators

  void setDamping(float linear, float angular);
  void setRestitution(float restitution);

  // instant
  void applyImpulse(glm::vec3 impulse,
                    glm::vec3 relativePosition = glm::vec3(0.0f));
  // time based
  void applyForce(glm::vec3 force,
                  glm::vec3 relativePosition = glm::vec3(0.0f));
  // lock/control displacement for certain directions
  void setLinearFactor(glm::vec3 factor);
  // lock/control rotating for certain axis
  void setAngularFactor(glm::vec3 factor);

  float getLinearDamping();
  float getAngularDamping();

  float getRestitution();

  glm::vec3 getLinearFactor();
  glm::vec3 getAngularFactor();

  glm::vec3 getLinearVelocity();
  glm::vec3 getAngularVelocity();

  RigidType getRigidType();

  btCollisionObject *getCollisionObjectHandle() override;
  Drawable *getDrawable();

  Transform getWorldTransform() override;

protected:
  glm::vec3 oldScale;
  RigidType rType;
  collision::CollisionShape *shape = nullptr;
  Drawable *drawable = nullptr;

  void scaleShape(glm::vec3 scale);

  virtual void transformChanged() override;

private:
  /// pointing to collisionObject member (reduce cast calls)
  btRigidBody *rigidBody = nullptr;
};