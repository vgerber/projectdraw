#pragma once

#include "GL/glew.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "Core/Util/Debug/logging.h"

enum ShaderType { VERTEX, FRAGMENT, GEOMETRY };

struct ShaderLayer {
  std::string path;
  ShaderType type;
};

class Shader {
public:
  std::vector<ShaderLayer> layers;

  Shader();

  GLuint getId();
  void load();
  void use();

private:
  GLuint programm_id;
};
