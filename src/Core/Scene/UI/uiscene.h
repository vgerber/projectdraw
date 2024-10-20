#pragma once

#include "../scene.h"

class UiScene : public Scene {
public:
	UiScene(float width, float height);

	virtual void draw(GLfloat delta) override;
	virtual void updatePhysics(GLfloat delta) override;
	virtual void reload(int width, int height) override;

protected:
	virtual void setup() override;
};