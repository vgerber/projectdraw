#include "arenderer.h"

int AbstractRenderer::globalRendererId = 0;

AbstractRenderer::AbstractRenderer(int width, int height) {
    this->width = width;
    this->height = height;
    rendererId = globalRendererId++;
}

void AbstractRenderer::resize(int width ,int height) {
    this->width = width;
    this->height = height;
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