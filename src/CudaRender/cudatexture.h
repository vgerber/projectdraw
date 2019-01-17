#pragma once
#include <Core/Texture/texture.h>



#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include <stdio.h>

//struct cudaGraphicsResource_t;
//struct uchar4;

class CudaTexture : public Texture
{
public:
	CudaTexture(int width, int height);
	~CudaTexture();

	void map();
	void unmap();

	virtual void activate(int tex_id) override;

	virtual GLuint getGLTexture() override;

	virtual uchar4 * getCudaTexture();
private:
	GLuint bufferID;
	cudaGraphicsResource *cuda_pbo_resource;
	cudaArray *cudaImageArray;
	uchar4 * d_dst;
	uchar4 * buffer_data = nullptr;

	
};

