#pragma once

#include "Core/Scene/Particle/2D/point_generator.h"

class BillboardGenerator2D : public PointGenerator2D {
public:
    BillboardGenerator2D(unsigned int particleCount);

    virtual void update(float delta) override;
};