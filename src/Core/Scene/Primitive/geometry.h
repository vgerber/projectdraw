#pragma once

#include "../drawable.h"

struct Point {
	glm::vec3 position;
};

class Geometry : public Drawable
{
public:
	glm::vec4 color;

	Geometry();
	~Geometry();
	void draw();
	void draw(Shader shader);
	void draw_normal();
	
	void lineTo(Point point);
	void lineTo(glm::vec3 point);
	void line(Point p1, Point p2);
	void line(glm::vec3 p1, glm::vec3 p2);

	void addPoint(Point point);
	void addPoint(glm::vec3 point);

	void removePoint(Point point);
	std::vector<Point> getPoints();

	void clear();
private:
	std::vector<Point> points;
	int size = 0;
	GLuint VAO, VBO;
};
