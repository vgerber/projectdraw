#include "arenderer.h"

int AbstractRenderer::globalRendererId = 0;

AbstractRenderer::AbstractRenderer(int width, int height, Camera &camera) {
	this->renderMode = RenderMode::FILLR;
    this->width = width;
    this->height = height;
    this->camera = &camera;
    rendererId = globalRendererId++;
}

void AbstractRenderer::resize(int width ,int height) {
    this->width = width;
    this->height = height;
}

void AbstractRenderer::setRenderMode(RenderMode renderMode) {
    this->renderMode = renderMode;
}

int AbstractRenderer::getWidth() {
    return width;
}

int AbstractRenderer::getHeight() {
    return height;
}

int AbstractRenderer::getRendererId() {
    return rendererId;
}