#include "glrecorder.h"

GLRecorder::GLRecorder(const char * file, unsigned int width, unsigned height, unsigned int quant_min, unsigned int quant_max, unsigned int fps) : Recorder(file, width, height)
{
	AVCodec *codec;
	int ret;
	avcodec_register_all();
	codec = avcodec_find_encoder(AV_CODEC_ID_MPEG1VIDEO);
	if (!codec) {
		printf("[RECORDER] [ERROR] Codec not found\n");
		exit(1);
	}
	m_ctx = avcodec_alloc_context3(codec);
	if (!m_ctx) {
		printf("[RECORDER] [ERROR] Could not allocate video codec context\n");
		exit(1);
	}


	m_ctx->bit_rate = 500000;
	m_ctx->width = width;
	m_ctx->height = height;
	m_ctx->time_base.num = 1;
	m_ctx->time_base.den = fps; //fps
	m_ctx->gop_size = 10;
	m_ctx->max_b_frames = 1;
	//c->qcompress = 1.0f;
	//c->qblur = 0.0f;
	m_ctx->qmax = quant_max;
	m_ctx->qmin = quant_min;
	m_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
	if (avcodec_open2(m_ctx, codec, NULL) < 0) {
		printf("[RECORDER] [ERROR] Could not open codec\n");
		exit(1);
	}
	m_frame = av_frame_alloc();
	if (!m_frame) {
		printf("[RECORDER] [ERROR] Could not allocate video frame\n");
		exit(1);
	}
	m_frame->format = m_ctx->pix_fmt;
	m_frame->width = m_ctx->width;
	m_frame->height = m_ctx->height;
	ret = av_image_alloc(m_frame->data, m_frame->linesize, m_ctx->width, m_ctx->height, m_ctx->pix_fmt, 32);
	if (ret < 0) {
		printf("[RECORDER] [ERROR] Could not allocate raw picture buffer\n");
		exit(1);
	}
}

void GLRecorder::captureFrame()
{
	readGLPixels();
	int ret, got_output;
	//convert rgb -> yuv
	//set frame
	convertRGBtoYUVFrame();
	av_init_packet(&m_pkt);
	m_pkt.data = NULL;
	m_pkt.size = 0;

	ret = avcodec_encode_video2(m_ctx, &m_pkt, m_frame, &got_output);
	if (ret < 0) {
		fprintf(stderr, "Error encoding frame\n");
		exit(1);
	}
	if (got_output) {
		fwrite(m_pkt.data, 1, m_pkt.size, m_file);
		av_packet_unref(&m_pkt);
	}
}

void GLRecorder::finish()
{
	uint8_t endcode[] = { 0, 0, 1, 0xb7 };
	int got_output, ret;
	do {
		fflush(stdout);
		ret = avcodec_encode_video2(m_ctx, &m_pkt, NULL, &got_output);
		if (ret < 0) {
			fprintf(stderr, "Error encoding frame\n");
			exit(1);
		}
		if (got_output) {
			fwrite(m_pkt.data, 1, m_pkt.size, m_file);
			av_packet_unref(&m_pkt);
		}
	} while (got_output);
	//write squence end code for real mpeg
	fwrite(endcode, 1, sizeof(endcode), m_file);
	fclose(m_file);
	avcodec_close(m_ctx);
	av_free(m_ctx);
	av_freep(&m_frame->data[0]);
	av_frame_free(&m_frame);
	free(m_rgb);
}

void GLRecorder::convertRGBtoYUVFrame()
{
	const int in_linesize[1] = { 4 * m_ctx->width };
	//get context for conversion (rgb -> yuv420p)
	m_sws_ctx = sws_getCachedContext(m_sws_ctx,
		m_ctx->width, m_ctx->height, AV_PIX_FMT_RGB32,
		m_ctx->width, m_ctx->height, AV_PIX_FMT_YUV420P,
		0, NULL, NULL, NULL);
	//apply conversion and set frame data
	sws_scale(m_sws_ctx, (const uint8_t * const *)&m_rgb, in_linesize, 0,
		m_ctx->height, m_frame->data, m_frame->linesize);
}

void GLRecorder::readGLPixels()
{
	size_t i, j, k, position_gl, position_rgb, nvals;
	const size_t format_nchannels = 4;
	//calc byte count
	nvals = format_nchannels * m_width * m_height;
	m_pixels = (GLubyte*)realloc(m_pixels, nvals * sizeof(GLubyte));
	m_rgb = (uint8_t*)realloc(m_rgb, nvals * sizeof(uint8_t));
	/* Get RGBA to align to 32 bits instead of just 24 for RGB. May be faster for FFmpeg. */
	glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, m_pixels);
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			position_gl = format_nchannels * (m_width * (m_height - i - 1) + j);
			position_rgb = format_nchannels * (m_width * i + j);
			for (k = 0; k < format_nchannels - 1; k++) {
				int k_offset = k;
				m_rgb[position_rgb + (k_offset)] = m_pixels[position_gl + k_offset];
			}
		}
	}
}
