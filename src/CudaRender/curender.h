#pragma once

#include <Windows.h>

/*
#define GLEW_STATIC
#include <GL/glew.h>
*/
void initCudaRender();

void cuMapBuffer(void ** devicePtr, unsigned int buffer);

void cuUnmapBuffer(unsigned int buffer);

void cuRegisterBuffer(unsigned int buffer);
