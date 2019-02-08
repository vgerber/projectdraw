#include "smaa.h"

void SMAA::apply(const Texture & texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, smaaFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_BLEND);

	//smaa edge detection
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSMAAEdge.use();
		//glUniform2f(glGetUniformLocation(shaderSMAAEdgeLuma.getId(), "screenSize"), 1.0f / WIDTH, 1.0f / HEIGHT);
		glUniform1i(glGetUniformLocation(shaderSMAAEdge.getId(), "albedoTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glCopyTextureSubImage2D(edgeTexture, 0, 0, 0, 0, 0, width, height);
	}

	//smaa weight calculation
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSMAAWeight.use();
		glUniform1i(glGetUniformLocation(shaderSMAAWeight.getId(), "edgeTexture"), 0);
		glUniform1i(glGetUniformLocation(shaderSMAAWeight.getId(), "areaTexture"), 1);
		glUniform1i(glGetUniformLocation(shaderSMAAWeight.getId(), "searchTexture"), 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, edgeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, areaTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, searchTexture);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glCopyTextureSubImage2D(weightTexture, 0, 0, 0, 0, 0, width, height);
	}
	//smaa blending
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSMAABlend.use();
		glUniform1i(glGetUniformLocation(shaderSMAABlend.getId(), "albedoTexture"), 0);
		glUniform1i(glGetUniformLocation(shaderSMAABlend.getId(), "blendTexture"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.getGLTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, weightTexture);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glCopyTextureSubImage2D(texture.getGLTexture(), 0, 0, 0, 0, 0, width, height);
	}
}

void SMAA::resize(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, smaaFBO);
	// create a color attachment texture

	glBindTexture(GL_TEXTURE_2D, smaaTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, smaaTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindTexture(GL_TEXTURE_2D, edgeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glBindTexture(GL_TEXTURE_2D, weightTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SMAA::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, smaaFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SMAA::dispose()
{
	glDeleteFramebuffers(1, &smaaFBO);

	glDeleteTextures(1, &edgeTexture);
	glDeleteTextures(1, &weightTexture);
	glDeleteTextures(1, &areaTexture);
	glDeleteTextures(1, &searchTexture);
}

void SMAA::setup()
{
	unsigned int smaaFBO;
	glGenFramebuffers(1, &smaaFBO);

	glGenTextures(1, &smaaTexture);
	glGenTextures(1, &edgeTexture);
	glGenTextures(1, &weightTexture);
	glGenTextures(1, &areaTexture);
	glGenTextures(1, &searchTexture);

	glBindTexture(GL_TEXTURE_2D, areaTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, AREATEX_WIDTH, AREATEX_HEIGHT, 0, GL_RG, GL_UNSIGNED_BYTE, areaTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, searchTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, searchTexBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	GLfloat screenRectangle[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	
	glGenVertexArrays(1, &screenRectVAO);
	glGenBuffers(1, &screenRectVBO);
	glBindVertexArray(screenRectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), &screenRectangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(float)));
	glBindVertexArray(0);
}
