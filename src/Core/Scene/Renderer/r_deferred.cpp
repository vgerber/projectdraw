#include "r_deferred.h"

DeferredRenderer::DeferredRenderer(int width, int height) : AbstractRenderer(width, height)
{
    setup();
    resize(width, height);
}

void DeferredRenderer::resize(int width, int height) {

}

void DeferredRenderer::setup() {
    
}