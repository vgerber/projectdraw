#pragma once

#include "flat_text.h"
#include "Core/Scene/Text/multiline_text.h"

class FlatMultilineText : public FlatText {
public:    
    FlatMultilineText(MultilineText * multilineTex);   

    virtual void update() override;
};