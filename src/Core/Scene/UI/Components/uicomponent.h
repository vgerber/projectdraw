#pragma once

#include "Core/Util/Transform/moveable.h"
#include "Core/Scene/drawable.h"

class UIComponent : public SceneObject {
public:

	virtual void update() = 0;

	virtual void setAction(int action) = 0;

	virtual Drawable * getDrawable() = 0;

	void enable(bool enable);
	void focus(bool focus);

	bool isEnabled();
	bool isFocused();
	
protected:
	bool enabled = true;
	bool focused = false;
};