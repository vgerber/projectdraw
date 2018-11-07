#pragma once

#include "arenderer.h"

class DeferredRenderer : public AbstractRenderer {
public:
    DeferredRenderer(int width, int height);

    virtual void resize(int width, int height);

private:
    void setup();

};