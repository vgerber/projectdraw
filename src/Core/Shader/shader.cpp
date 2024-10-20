#include "shader.h"

Shader::Shader() {}

GLuint Shader::getId() { return programm_id; }

void Shader::load() {
  GLint result = GL_FALSE;
  int info_length;
  std::vector<GLuint> shader_ids;
  for (ShaderLayer layer : layers) {
    GLuint shader_id = 0;
    if (layer.type == ShaderType::VERTEX) {
      shader_id = glCreateShader(GL_VERTEX_SHADER);
    } else if (layer.type == ShaderType::FRAGMENT) {
      shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
      shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    }
    std::string code;
    std::ifstream shader_stream(layer.path, std::ios::in);
    if (shader_stream.is_open()) {
      std::stringstream sstr;
      sstr << shader_stream.rdbuf();
      code = sstr.str();
      shader_stream.close();
    } else {
      Log::write(LogType::Error, "Impossible to open " + layer.path, "Shader");
      getchar();
      return;
    }

    // Compile Vertex Shader
    Log::write(LogType::Info, "Compiling shader " + layer.path, "Shader");
    char const *code_ptr = code.c_str();
    glShaderSource(shader_id, 1, &code_ptr, NULL);
    glCompileShader(shader_id);

    // Check Vertex Shader
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_length);
    if (info_length > 0) {
      std::vector<char> error_msg(info_length + 1);
      glGetShaderInfoLog(shader_id, info_length, NULL, &error_msg[0]);
      Log::write(LogType::Error, std::string(&error_msg[0]), "Shader");
    }
    shader_ids.push_back(shader_id);
  }
  Log::write(LogType::Info, "Linking program", "Shader");
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
    // printf("%s\n", &program_err_msg[0]);
    Log::write(LogType::Error, std::string(program_err_msg.data()), "Shader");
  }

  for (GLuint shader_id : shader_ids) {
    glDetachShader(programm_id, shader_id);
  }

  for (GLuint shader_id : shader_ids) {
    glDeleteShader(shader_id);
  }
}

void Shader::use() { glUseProgram(programm_id); }
