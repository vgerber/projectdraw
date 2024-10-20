#pragma once

#include <functional>
#include <random>

#include "Core/Scene/Animation/animatable.h"
#include "Core/Scene/drawable.h"

class ParticleGenerator : public Animatable, public Drawable {
public:
  /**
   * @brief Change particle count
   *
   * @param particleCount
   */
  void setParticleCount(unsigned int particleCount);

  void setLifeTime(float lifeTime);

  /**
   * @brief True if generator should generate particles
   *
   * @param enabled
   */
  void enable(bool enabled);

  /**
   * @brief Resets particles to inital state
   *
   */
  virtual void reset() = 0;

  virtual Size getSize() override;

protected:
  float particleLifetime = 2.0;
  unsigned int particleCount = 0;
  float particlesPerMillisecond = 0;
  float queuedParticles = 0;
  bool active = false;
};