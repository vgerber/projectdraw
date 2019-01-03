#pragma once

#include "Core/Scene/drawable.h"

struct Point {
	glm::vec3 position;
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class Geometry : public Drawable
{
public:
	int lineThickness = 1;
	int pointThickness = 1;
	glm::vec4 color;

	Geometry();
	Geometry(std::vector<Point> points);

	Size getSize();
	
	void lineTo(Point point);
	void lineTo(glm::vec3 point);
	void lineTo(std::vector<Point> points);
	void line(Point p1, Point p2);
	void line(glm::vec3 p1, glm::vec3 p2);

	void addPoint(Point point);
	void addPoint(glm::vec3 point);

	void removePoint(Point point);
	std::vector<Point> getPoints();

	void clear();
protected:
	std::vector<Point> points;
	int size = 0;
	GLuint VAO, VBO;


	///draw meshes + submeshes
	virtual void drawModel(Shader shader, DrawType type) override;

	///instancing  meshes + submeshes
	virtual void drawModelInstancing(Shader shader, DrawType type, int amount) override;

	///draw mesh normals + submesh normals
	virtual void drawModelNormals(Shader shader) override;

	///draw mesh bounding box
	virtual void drawModelBox(Shader shader) override;

	virtual void setup();
};
