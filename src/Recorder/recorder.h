#pragma once

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>

	class Recorder {
	public:
		Recorder(const char * file, unsigned int width, unsigned int height);

		virtual void captureFrame() = 0;
		virtual void finish() = 0;

	protected:
		unsigned int m_width;
		unsigned int m_height;

		AVCodecContext *m_ctx = NULL;
		AVFrame *m_frame;
		AVPacket m_pkt;
		FILE *m_file;
		struct SwsContext *m_sws_ctx = NULL;
	};

}