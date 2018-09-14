#pragma once

#include <bullet/LinearMath/btIDebugDraw.h>
#include <GL/glew.h>

#include "../../Scene/scene.h"

class BulletDebugDrawer : public btIDebugDraw {
public:
    BulletDebugDrawer();

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    virtual void drawContactPoint(const btVector3 &pointA, const btVector3 &pointB, btScalar distance, int lifeTime, const btVector3 &color) override;

    virtual void reportErrorWarning(const char* warningString) override;

    virtual void draw3dText(const btVector3& location, const char *textString);

    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const { return m_debugMode; };

    void clear();
    void draw();
private:
    int m_debugMode;
    Geometry m_lineGeometry;
    Geometry m_pointGeometry;

    
}