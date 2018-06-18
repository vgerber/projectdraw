#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "sceneobject.h"
#include "../Model/model.h"
#include "../Model/box.h"
#include <memory>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

class Drawable : public SceneObject, public BoundingBox
{
public:
	Drawable();
	Drawable(GLchar* mmodel);
	~Drawable();

	bool visible_normal = false;
	bool visible_box = false;
	DrawType draw_type = DrawType::TRIANGLE;

	virtual void dispose();
	virtual void draw();
	virtual void draw(Shader shader);
	virtual void draw_normals();
	virtual void draw_normals(Shader shader);
	virtual void draw_box();
	virtual void draw_box(Shader shader);
	

	glm::mat4 get_model_matrix();

	void rotate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(glm::vec3 vrotation);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translate(glm::vec3 vtranslation);
	void scale(GLfloat x, GLfloat y, GLfloat z);
	void scale(glm::vec3 vscale);

	Size get_size();
	void scale_to_size(Size size);
	void scale_to_width(GLfloat width);
	void scale_to_height(GLfloat height);
	void scale_to_length(GLfloat depth);

	glm::vec3 get_position();
	glm::vec3 get_scale();
	glm::vec3 get_rotation();
	void set_position(glm::vec3 position);

	void set_center(glm::vec3 center);

	Model get_model();
	void set_model(Model model);

	void set_shader(Shader shader);
	unsigned int get_dimension();
protected:
	GLuint boxVBO, boxVAO;
	Size size;
	Model objModel;
	Shader shader;
	Shader shader_normals;
	Shader shader_box;

	glm::vec3 vcenter = glm::vec3(0.5f);
	glm::vec3 vrotation = glm::vec3(0.0f);
	glm::vec3 vscale = glm::vec3(1.0f);
	glm::mat4 mmodel = glm::mat4(1.0f);

	virtual void setup();
	virtual void update_model();
	virtual void load_box();
private:
	bool size_changed = true;
	unsigned int dimension = 0;
};
