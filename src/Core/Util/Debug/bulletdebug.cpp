#include "bulletdebug.h"


BulletDebugDrawer::BulletDebugDrawer() {
    m_debugMode = 0;
}

void BulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    Point pA;
    Point pB;

    pA.position = toVec3(from);
    pA.color = glm::vec4(toVec3(color), 1.0f);

    pB.position = toVec3(to);
    pB.color = glm::vec4(toVec3(color), 1.0f);

    m_lineGeometry.line(pA, pB);  

}

void BulletDebugDrawer::drawContactPoint(const btVector3 &from, const btVector3 &to, btScalar distance, int lifeTime, const btVector3 &color) {
    Point pA;
    Point pB;

    pA.position = toVec3(from);
    pA.color = glm::vec4(toVec3(color), 1.0f);

    pB.position = toVec3(to);
    pB.color = glm::vec4(toVec3(color), 1.0f);

    m_pointGeometry.addPoint(pA);
    m_pointGeometry.addPoint(pB);
}

void BulletDebugDrawer::reportErrorWarning(const char *warningString) {
    printf(warningString);
}

void BulletDebugDrawer::draw3dText(const btVector3 &location, const char *textString) {

}

void BulletDebugDrawer::setDebugMode(int debugMode) {
    m_debugMode = debugMode;
}

void BulletDebugDrawer::clear() {
    m_lineGeometry.clear();
    m_pointGeometry.clear();
}

void BulletDebugDrawer::draw() {
    m_lineGeometry.draw();
    m_pointGeometry.draw();
}