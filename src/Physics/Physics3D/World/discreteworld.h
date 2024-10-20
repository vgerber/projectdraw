#pragma once

#include <vector>

#include "Physics/Physics3D/CollisionObject/Rigidbody/rigidbody.h"
#include "Physics/Physics3D/CollisionObject/Trigger/triggervolume.h"
#include "Physics/Physics3D/CollisionObject/Vehicle/raycastvehicle.h"
#include "Physics/Physics3D/World/collisionworld.h"

#include "../Util/debugdrawer.h"
#include "LinearMath/btIDebugDraw.h"

class RaycastVehicle;

class DiscreteWorld : public CollisionWorld {
public:
  DiscreteWorld();

  virtual void addCollisionObject(CollisionObject &collisionObject, int group,
                                  int mask) override;
  virtual void removeCollisionObject(CollisionObject &collisionObject) override;

  virtual void update(float delta);

  void setQuality(int maxSteps, float fixedTimeStep);

private:
  int maxSimulationSteps = 10;
  float fixedSimulationStep = 1.0f / 60.0f;
  std::vector<RigidBody *> rigidBodies;
  std::vector<TriggerVolume *> triggerVolumes;
  std::vector<RaycastVehicle *> raycastVehicles;

  btSequentialImpulseConstraintSolver *constrainSolver = nullptr;

  virtual void setup() override;
};