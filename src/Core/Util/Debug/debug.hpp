#pragma once

#include <GL/glew.h>

#define GLcheckError() {GLenum err;while((err = glGetError()) != GL_NO_ERROR){std::cout << "OpenGL Error in " << __FILE__ << " Line " << __LINE__ << " - " << err << std::endl;}}