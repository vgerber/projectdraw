#include "geometry.h"

Geometry::Geometry()
{
	shader = Shaders[SHADER_DEFFERED_GEOMETRY];
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (GLvoid*)0);
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
}

void Geometry::draw()
{
	draw(shader);
}

void Geometry::draw(Shader shader)
{
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	glBindVertexArray(VAO);
	if (size != points.size()) {		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), &points[0], GL_STATIC_DRAW);	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (GLvoid*)0); 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Point), (GLvoid*)offsetof(Point, color));
		size = points.size();
	}
	if (points.size() > 0) {
		if (drawType == DrawType::LINE) {
			glDrawArrays(GL_LINES, 0, points.size());
		}
		else {
			glDrawArrays(GL_POINTS, 0, points.size());
		}
	}
	glBindVertexArray(0);
}

void Geometry::draw_normal()
{
	shader_normals.use();
}

void Geometry::lineTo(Point point)
{
	if (points.size() % 2 == 0) {
		points.push_back(point);
	}
	else {
		points.push_back(point);
		points.push_back(point);
	}
}

void Geometry::lineTo(glm::vec3 point)
{
	lineTo(Point { point, color });
}

void Geometry::line(Point p1, Point p2)
{
	if (points.size() % 2 == 1) {
		points.push_back(points[points.size() - 1]);
	}
	points.push_back(p1);
	points.push_back(p2);
}

void Geometry::line(glm::vec3 p1, glm::vec3 p2)
{
	line(Point{ p1, color }, Point{ p2, color });
}

void Geometry::addPoint(Point point)
{
	points.push_back(point);
}

void Geometry::addPoint(glm::vec3 point)
{
	addPoint(Point { point, color });
}

void Geometry::removePoint(Point point)
{
	for (size_t i = 0; i < points.size(); i++) {
		if (points[i].position == point.position) {
			points.erase(points.begin() + i);
			i--;
		}
	}
}

std::vector<Point> Geometry::getPoints()
{
	return points;
}

void Geometry::clear() {
	points.clear();
	size = 0;
}