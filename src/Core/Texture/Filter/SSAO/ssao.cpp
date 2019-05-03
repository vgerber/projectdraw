#include "ssao.h"

void SSAOFilter::apply(const Camera &camera, unsigned int normals, unsigned int positions) {
    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    //get ssao color
    shaderSSAO.use();
    /*glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "texNoise"), 2);    
    for(int i = 0; i < kernel.size(); i++) {
        glUniform3f(glGetUniformLocation(shaderSSAO.getId(), ("samples[" + std::to_string(i) + "]").c_str()), kernel[i].x, kernel[i].y, kernel[i].z);
    }
    glUniform2f(glGetUniformLocation(shaderSSAO.getId(), "noiseScale"), noiseScale.x, noiseScale.y);*/
    glUniformMatrix4fv(glGetUniformLocation(shaderSSAO.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, positions);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normals);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);

    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glCopyTextureSubImage2D(ssaoTexture, 0, 0, 0, 0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    //blur ssao image
    shaderSSAOBlur.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoTexture);

    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

void SSAOFilter::resize(int width, int height) {
    this->width = width;
    this->height = height;
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glBindTexture(GL_TEXTURE_2D, ssaoFinalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoFinalTexture, 0);  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, ssaoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    noiseScale.x = (float)width  / 4.0;
    noiseScale.y = (float)height / 4.0;

    shaderSSAO.use();
    glUniform2f(glGetUniformLocation(shaderSSAO.getId(), "noiseScale"), noiseScale.x, noiseScale.y);
}

unsigned int SSAOFilter::getGLTexture() {
    return ssaoFinalTexture;
}

void SSAOFilter::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SSAOFilter::dispose() {
    glDeleteFramebuffers(1, &ssaoFBO);
    glDeleteTextures(1, &ssaoTexture);
    glDeleteTextures(1, &ssaoFinalTexture);
    glDeleteTextures(1, &noiseTexture);
}

void SSAOFilter::setup() {
    //generate normal orientations (hemisphere)
    std::uniform_real_distribution<float> randomFLoats(0.0, 1.0);
    std::default_random_engine generator;
    for(int i = 0; i < 64; i++) {
        glm::vec3 sample(
            randomFLoats(generator) * 2.0 - 1.0,
            randomFLoats(generator) * 2.0 - 1.0,
            randomFLoats(generator) //hemisphere (only in + direction)
        );
        sample = glm::normalize(sample);
        sample *= randomFLoats(generator);

        float scale = (float)i/64;
        scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
        sample *= scale;

        kernel.push_back(sample);
    }

    for(int i = 0; i < 16; i++) {
        glm::vec3 singleNoise(
            randomFLoats(generator) * 2.0 - 1.0,
            randomFLoats(generator) * 2.0 - 1.0,
            0.0
        );
        noise.push_back(singleNoise);
    }

    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &ssaoFBO);
    glGenTextures(1, &ssaoFinalTexture);
    glGenTextures(1, &ssaoTexture);


    float rectangle[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
	};
	
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), &rectangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glBindVertexArray(0);

    shaderSSAO = ResourceManager::loadShader(ShaderName::Postprocessing::SSAO::SSAO);
    shaderSSAOBlur = ResourceManager::loadShader(ShaderName::Postprocessing::SSAO::SSAOBlur);

    
    shaderSSAO.use();
    glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderSSAO.getId(), "texNoise"), 2);    
    for(int i = 0; i < kernel.size(); i++) {
        glUniform3f(glGetUniformLocation(shaderSSAO.getId(), ("samples[" + std::to_string(i) + "]").c_str()), kernel[i].x, kernel[i].y, kernel[i].z);
    }

    shaderSSAOBlur.use();
    glUniform1i(glGetUniformLocation(shaderSSAOBlur.getId(), "ssaoInput"), 0);
}