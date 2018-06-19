#pragma once

#include "../core.h"
#include "../Model/box.h"
#include "../Scene/drawable.h"

struct AABB {
	Size box;
	Drawable *drawable;
};

struct EndPoint {
	int m_data;
	float m_value;
};

class SweepAndPrune {
	void add_object(const AABB &box);

	void update_object(const AABB &box);
};