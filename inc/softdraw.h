#include <inttypes.h>

#ifndef SOFTDRAW_H
#define SOFTDRAW_H
typedef struct {
	uint8_t r, g, b;
} Color_t;

typedef struct {
	uint32_t sizeX, sizeY;
	Color_t *buffer;
} FrameBuffer_t;

FrameBuffer_t *sd_NewFrameBuffer(const uint32_t x, const uint32_t y);
void sd_ClearBuffer(FrameBuffer_t *f, const Color_t c);
void sd_DrawLine(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c);
void sd_DrawPixel(FrameBuffer_t *f, const uint32_t x, const uint32_t y, const Color_t c);
void sd_FreeFrameBuffer(FrameBuffer_t *f);
#endif
