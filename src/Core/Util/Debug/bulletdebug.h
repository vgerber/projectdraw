#pragma once

#include <LinearMath/btIDebugDraw.h>

#include "../../Scene/Primitive/primitives.h"

class BulletDebugDrawer : public btIDebugDraw {
public:
    BulletDebugDrawer();

    virtual void drawLine(
		const btVector3 &from, 
		const btVector3 &to, 
		const btVector3 &color
	) override;

    virtual void drawContactPoint(
		const btVector3 &pointA, 
		const btVector3 &pointB,
		btScalar distance, 
		int	lifeTime, 
		const btVector3 &color
	) override;

    virtual void reportErrorWarning(const char* warningString) override;

    virtual void draw3dText(const btVector3& location, const char *textString) override;

    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const override;

	virtual void drawTriangle(
		const btVector3 &v0,
		const btVector3 &v1,
		const btVector3 &v2,
		const btVector3 &color,
		btScalar
	) override;

	virtual void drawTriangle(
		const btVector3 &v0,
		const btVector3 &v1,
		const btVector3 &v2,
		const btVector3&,
		const btVector3&,
		const btVector3&,
		const btVector3 &color,
		btScalar alpha
	) override;

    void clear();
    void draw();
private:
    int m_debugMode = 0;
    Geometry m_lineGeometry;
    Geometry m_pointGeometry;

    
};