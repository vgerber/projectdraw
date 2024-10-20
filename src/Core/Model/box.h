#pragma once
#include "Core/Mesh/Util/box.h"

class BoundingBox {
  virtual Size getSize() = 0;
  virtual void scaleToSize(Size size) = 0;
  virtual void scaleToWidth(float width) = 0;
  virtual void scaleToHeight(float height) = 0;
  virtual void scaleToLength(float depth) = 0;
};