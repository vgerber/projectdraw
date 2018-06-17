#pragma once

#include "../drawable.h"

struct Point {
	glm::vec3 position;
};

class Geometry : public Drawable
{
public:
	Geometry();
	~Geometry();
	void draw();
	void draw(Shader shader);
	void draw_normal();
	
	void line_to(Point point);
	void line_to(glm::vec3 point);

	void add_point(Point point);
	void add_point(glm::vec3 point);

	void remove_point(Point point);
	std::vector<Point> get_points();
private:
	std::vector<Point> points;
	int size = 0;
	GLuint VAO, VBO;
};
