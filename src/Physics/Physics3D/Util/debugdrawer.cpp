#include "debugdrawer.h"

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    geometry.color = glm::vec4(toGLMVec3(color), 1.0);
    geometry.line(toGLMVec3(from), toGLMVec3(to));
    geometry.settings.drawType = DrawType::LINEG;
}

void DebugDrawer::reportErrorWarning(const char * warningString) {
    printf("[Bullet] [Warning] %s\n", warningString);
}

void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)  {
	geometry.color = glm::vec4(toGLMVec3(color), 1.0);
	geometry.line(toGLMVec3(PointOnB), toGLMVec3(normalOnB * distance));
	geometry.settings.drawType = DrawType::LINEG;
}

void DebugDrawer::draw3dText(const btVector3 &location, const char * textString) {

}

void DebugDrawer::setDebugMode(int debugMode) {
    this->debugMode = debugMode;
}

int DebugDrawer::getDebugMode() const {
    return debugMode;
}

void DebugDrawer::clearLines() {
    geometry.clear();
}

Drawable * DebugDrawer::getDebugDrawable() {
    return &geometry;
}