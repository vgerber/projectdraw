#pragma once

#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <vector>

#include "Core/Scene/drawable.h"
#include "Physics/Physics3D/CollisionObject/collisionobject.h"
#include "Physics/Physics3D/Util/conversion.h"
#include "Physics/Physics3D/Util/debugdrawer.h"

struct RaycastHitResult {
  glm::vec3 castStartLocation;
  glm::vec3 castEndLocation;
  std::vector<std::pair<glm::vec3, const CollisionObject *>> hitLocations;
  bool hasHit = false;
};

class CollisionWorld {
public:
  CollisionWorld();

  virtual void addCollisionObject(CollisionObject &collisionObject, int group,
                                  int mask);
  virtual void removeCollisionObject(CollisionObject &collisionObject);

  virtual void updateCollision();

  std::vector<CollisionResult> getCollisions();

  RaycastHitResult rayCastAll(glm::vec3 from, glm::vec3 to,
                              bool approximate = false);

  RaycastHitResult rayCast(glm::vec3 from, glm::vec3 to);

  btCollisionWorld *getWorldHandle();

  // do not manipulate them outside!
  Drawable *getDebugDrawable();

protected:
  btCollisionWorld *world;
  std::vector<CollisionObject *> collisionObjects;
  DebugDrawer debugDrawer;

  // World configuration
  btDefaultCollisionConfiguration *collisionConfig = nullptr;
  btCollisionDispatcher *collisionDispatcher = nullptr;
  btBroadphaseInterface *broadphase = nullptr;

  virtual void setup();
};