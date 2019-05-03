#pragma once

#include "Core/Scene/Particle/3D/point_generator.h"

class BillboardGenerator3D : public PointGenerator3D {
public:
    BillboardGenerator3D(unsigned int particleCount);

    virtual void update(float delta) override;
};