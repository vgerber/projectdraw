#pragma once
#include <GL/glew.h>

extern "C" {
	#include "recorder.h"
	class GLRecorder : public Recorder {
	public:
		GLRecorder(const char * file, unsigned int width, unsigned height, unsigned int quant_min = 1, unsigned int quant_max = 10, unsigned int fps = 25);

		void captureFrame() override;
		void finish() override;

	private:
		uint8_t *m_rgb = NULL;
		GLubyte *m_pixels = NULL;

		void convertRGBtoYUVFrame();
		void readGLPixels();
	};
}