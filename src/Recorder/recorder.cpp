#include "recorder.h"

Recorder::Recorder(const char * file, unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_file = fopen(file, "wb");
	if (!file) {
		printf("[RECORDER] [ERROR] Could not open %s\n", file);
		exit(1);
	}
}
