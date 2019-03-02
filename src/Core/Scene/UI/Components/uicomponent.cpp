#include "uicomponent.h"

void UIComponent::enable(bool enable)
{
	this->enabled = enable;
}

void UIComponent::focus(bool focus)
{
	this->focused = true;
}

bool UIComponent::isEnabled()
{
	return enabled;
}

bool UIComponent::isFocused()
{
	return focused;
}
