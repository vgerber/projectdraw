#pragma once
#include <vector>
#include <stdio.h>

#include <LinearMath/btIDebugDraw.h>

#include "Core/Scene/Primitive/primitives.h"
#include "Physics/Physics3D/Util/conversion.h"

class DebugDrawer : public btIDebugDraw {
public:
    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    virtual void reportErrorWarning(const char * warningString) override;

    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;

    virtual void draw3dText(const btVector3 &location, const char * textString) override;

    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const override;

    virtual void clearLines() override;

    Drawable * getDebugDrawable();

private:
    int debugMode = DBG_NoDebug;
    Geometry geometry;
};