#include "r_deferred.h"

DeferredRenderer::DeferredRenderer(int width, int height, const Camera &camera) : AbstractRenderer(width, height, camera)
{
    setup();
    resize(width, height);
}

void DeferredRenderer::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void DeferredRenderer::resize(int width, int height) {

}

void DeferredRenderer::setup() {

}

void DeferredRenderer::beginScene() {
    glViewport(0, 0, getWidth(), getHeight());
    glEnable(GL_DEPTH_TEST);    
}

void DeferredRenderer::endScene() {

}

void DeferredRenderer::beginObjects() {
    int polygonMode = GL_FILL;
	if (renderMode == RenderMode::POINTR)
		polygonMode = GL_POINT;
	else if (renderMode == RenderMode::LINER)
		polygonMode = GL_LINE;
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void DeferredRenderer::endObjects() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_STENCIL_TEST);
}

void DeferredRenderer::beginLight(int lightType) {

}

void DeferredRenderer::endLight() {

}