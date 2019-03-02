#pragma once

#include "Core/Scene/UI/Components/uicomponent.h"
#include "Core/Scene/Text/text.h"
#include "Core/Resource/resourcemanager.h"

class TextBlock : public UIComponent {
public:
	TextBlock();

	virtual void update() override;

	virtual void setAction(int action) override;

	virtual Drawable * getDrawable() override;

	virtual void setText(std::string text);

protected:
	Text * textDrawable;
};