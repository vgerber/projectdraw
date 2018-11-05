#include "curender.h"
#include "cuda_gl_interop.h"


void initCudaRender()
{
	cudaGLSetGLDevice(0);
}

void cuMapBuffer(void ** devicePtr, unsigned int buffer)
{
	cudaGLMapBufferObject(devicePtr, buffer);
}

void cuUnmapBuffer(unsigned int buffer)
{
	cudaGLUnmapBufferObject(buffer);
}

void cuRegisterBuffer(unsigned buffer)
{
	cudaGLRegisterBufferObject(buffer);
}

