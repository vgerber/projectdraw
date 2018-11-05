#include "CudaTexture.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

CudaTexture::CudaTexture(int width, int height)
{
	//8bit x 4 channels
	buffer_data = new uchar4[width * height];

	printf("Creating GL texture...\n");
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer_data);
	printf("Texture created.\n");


	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);
	//8bit x 4 channels
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, buffer_data, GL_STREAM_COPY);

	m_width = width;
	m_height = height;
	m_nrChannels = 4;

	
	cudaError result = cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, bufferID, cudaGraphicsMapFlagsWriteDiscard);
	if (result != cudaSuccess) {
		printf("[CUDA] [ERROR] can't register buffer\n");
	}
}

CudaTexture::~CudaTexture()
{
}

void CudaTexture::map()
{
	cudaError error = cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
	if (error != cudaSuccess) {
		printf("[CUDA] [ERROR] mapping failed\n");
	}
	size_t num_bytes;
	error = cudaGraphicsResourceGetMappedPointer((void**)&d_dst, &num_bytes, cuda_pbo_resource);
	if (error != cudaSuccess) {
		printf("[CUDA] [ERROR] ptr mapping failed\n");
	}
	
}

void CudaTexture::unmap()
{
	cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);
}

void CudaTexture::activate(int tex_id)
{
	Texture::activate(tex_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

GLuint CudaTexture::getGLTexture()
{
	return m_texture;
}

uchar4 * CudaTexture::getCudaTexture()
{
	return d_dst;
}
