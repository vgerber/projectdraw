#include "collisionworld.h"

CollisionWorld::CollisionWorld() { setup(); }

void CollisionWorld::addCollisionObject(CollisionObject &collisionObject,
                                        int group, int mask) {
  collisionObjects.push_back(&collisionObject);
}

void CollisionWorld::removeCollisionObject(CollisionObject &collisionObject) {
  for (int i = 0; i < collisionObjects.size(); i++) {
    if (collisionObjects[i] == &collisionObject) {
      collisionObjects.erase(collisionObjects.begin() + i);
      i--;
    }
  }
}

void CollisionWorld::updateCollision() {
  world->performDiscreteCollisionDetection();
  int numMainifolds = world->getDispatcher()->getNumManifolds();
  for (int i = 0; i < numMainifolds; i++) {
    btPersistentManifold *contactManifold =
        world->getDispatcher()->getManifoldByIndexInternal(i);
    CollisionObject *objectA = static_cast<CollisionObject *>(
        contactManifold->getBody0()->getUserPointer());
    CollisionObject *objectB = static_cast<CollisionObject *>(
        contactManifold->getBody1()->getUserPointer());
    if (objectA->onCollide) {
      objectA->onCollide(objectB);
    }
    if (objectB->onCollide) {
      objectB->onCollide(objectA);
    }
  }
}

std::vector<CollisionResult> CollisionWorld::getCollisions() {
  std::vector<CollisionResult> collisions;

  int numMainifolds = world->getDispatcher()->getNumManifolds();
  collisions = std::vector<CollisionResult>(numMainifolds);
  for (int i = 0; i < numMainifolds; i++) {
    CollisionResult collision;
    btPersistentManifold *contactManifold =
        world->getDispatcher()->getManifoldByIndexInternal(i);
    collision.objectA = static_cast<CollisionObject *>(
        contactManifold->getBody0()->getUserPointer());
    collision.objectB = static_cast<CollisionObject *>(
        contactManifold->getBody1()->getUserPointer());
    collision.collisionPointsA =
        std::vector<glm::vec3>(contactManifold->getNumContacts());
    collision.collisionPointsB =
        std::vector<glm::vec3>(contactManifold->getNumContacts());
    for (int j = 0; j < contactManifold->getNumContacts(); j++) {
      btManifoldPoint &pt = contactManifold->getContactPoint(j);
      collision.collisionPointsA[j] = toGLMVec3(pt.getPositionWorldOnA());
      collision.collisionPointsB[j] = toGLMVec3(pt.getPositionWorldOnB());
    }
    collisions[i] = collision;
  }

  return collisions;
}

RaycastHitResult CollisionWorld::rayCastAll(glm::vec3 from, glm::vec3 to,
                                            bool approximate) {
  btCollisionWorld::AllHitsRayResultCallback result(toBtVec3(from),
                                                    toBtVec3(to));
  if (approximate) {
    result.m_flags |=
        btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
  }
  world->rayTest(toBtVec3(from), toBtVec3(to), result);
  RaycastHitResult hResult;
  hResult.castStartLocation = from;
  hResult.castEndLocation = to;
  hResult.hasHit = result.hasHit();
  if (result.hasHit()) {
    glm::vec3 directionVec = to - from;
    for (int i = 0; i < result.m_hitFractions.size(); i++) {
      glm::vec3 hitLocation = from + directionVec * result.m_hitFractions[i];
      CollisionObject *hitCollisionObject = nullptr;
      hitCollisionObject = static_cast<CollisionObject *>(
          result.m_collisionObjects[i]->getUserPointer());
      hResult.hitLocations.push_back(
          std::make_pair(hitLocation, hitCollisionObject));
    }
  }
  return hResult;
}

RaycastHitResult CollisionWorld::rayCast(glm::vec3 from, glm::vec3 to) {
  btCollisionWorld::ClosestRayResultCallback result(toBtVec3(from),
                                                    toBtVec3(to));
  result.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
  world->rayTest(toBtVec3(from), toBtVec3(to), result);
  RaycastHitResult hResult;
  hResult.castStartLocation = from;
  hResult.castEndLocation = to;
  hResult.hasHit = result.hasHit();
  if (result.hasHit()) {
    glm::vec3 directionVec = to - from;
    glm::vec3 hitLocation = from + directionVec * result.m_closestHitFraction;
    CollisionObject *hitCollisionObject = static_cast<CollisionObject *>(
        result.m_collisionObject->getUserPointer());
    hResult.hitLocations.push_back(
        std::make_pair(hitLocation, hitCollisionObject));
  }
  return hResult;
}

btCollisionWorld *CollisionWorld::getWorldHandle() { return world; }

Drawable *CollisionWorld::getDebugDrawable() {
  return debugDrawer.getDebugDrawable();
}

void CollisionWorld::setup() {
  collisionConfig = new btDefaultCollisionConfiguration();
  collisionDispatcher = new btCollisionDispatcher(collisionConfig);
  broadphase = new btDbvtBroadphase();
  world =
      new btCollisionWorld(collisionDispatcher, broadphase, collisionConfig);
}