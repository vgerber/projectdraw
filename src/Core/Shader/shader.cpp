#include "shader.h"

Shader::Shader()
{
}

GLuint Shader::getId()
{
	return programm_id;
}

void Shader::load()
{
	GLint result = GL_FALSE;
	int info_length;
	std::vector<GLuint> shader_ids;
	for (ShaderLayer layer : layers) {
		GLuint shader_id = 0;
		if(layer.type == ShaderType::VERTEX) {
			shader_id = glCreateShader(GL_VERTEX_SHADER);
		}
		else if (layer.type == ShaderType::FRAGMENT) {
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		}
		else {
			shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		}
		std::string code;
		std::ifstream shader_stream(layer.path, std::ios::in);
		if (shader_stream.is_open()) {
			std::stringstream sstr;
			sstr << shader_stream.rdbuf();
			code = sstr.str();
			shader_stream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ?\n", layer.path);
			getchar();
			return;
		}

		// Compile Vertex Shader
		printf("Compiling shader : %s\n", layer.path.c_str());
		char const * code_ptr = code.c_str();
		glShaderSource(shader_id, 1, &code_ptr, NULL);
		glCompileShader(shader_id);

		// Check Vertex Shader
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0) {
			std::vector<char> error_msg(info_length + 1);
			glGetShaderInfoLog(shader_id, info_length, NULL, &error_msg[0]);
			printf("%s\n", &error_msg[0]);
		}
		shader_ids.push_back(shader_id);
	}
	printf("Linking program\n");
	programm_id = glCreateProgram();
	for (GLuint shader_id : shader_ids) {
		glAttachShader(programm_id, shader_id);
	}
	glLinkProgram(programm_id);

	glGetProgramiv(programm_id, GL_LINK_STATUS, &result);
	glGetProgramiv(programm_id, GL_INFO_LOG_LENGTH, &info_length);
	if (info_length > 0) {
		std::vector<char> program_err_msg(info_length + 1);
		glGetProgramInfoLog(programm_id, info_length, NULL, &program_err_msg[0]);
		printf("%s\n", &program_err_msg[0]);
	}

	for (GLuint shader_id : shader_ids) {
		glDetachShader(programm_id, shader_id);
	}

	for (GLuint shader_id : shader_ids) {
		glDeleteShader(shader_id);
	}

}

void Shader::use()
{
	glUseProgram(programm_id);
}
