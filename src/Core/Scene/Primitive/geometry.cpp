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
	glUniform4f(glGetUniformLocation(shader.get_id(), "color"), 1.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	glBindVertexArray(VAO);
	if (size != points.size()) {		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), &points[0], GL_STATIC_DRAW);	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (GLvoid*)0);
		size = points.size();
	}
	if (points.size() > 0) {
		if (draw_type == DrawType::LINE) {
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

void Geometry::line_to(Point point)
{
	if (points.size() % 2 == 0) {
		points.push_back(point);
	}
	else {
		points.push_back(point);
		points.push_back(point);
	}
}

void Geometry::line_to(glm::vec3 point)
{
	line_to(Point { point });
}

void Geometry::add_point(Point point)
{
	points.push_back(point);
}

void Geometry::add_point(glm::vec3 point)
{
	add_point(Point { point });
}

void Geometry::remove_point(Point point)
{
	for (size_t i = 0; i < points.size(); i++) {
		if (points[i].position == point.position) {
			points.erase(points.begin() + i);
			i--;
		}
	}
}

std::vector<Point> Geometry::get_points()
{
	return points;
}
