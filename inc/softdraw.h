#ifndef SOFTDRAW_H
#define SOFTDRAW_H
#include <inttypes.h>

typedef struct {
	uint8_t r, g, b;
} Color_t;

typedef struct {
	int32_t v0[2];
	int32_t v1[2];
	int32_t v2[2];
} Tri_t;

#include "softdraw_image.h"


typedef struct {
	int32_t sizeX, sizeY;
	Color_t *buffer;
} FrameBuffer_t;

FrameBuffer_t *sd_NewFrameBuffer(const int32_t x, const int32_t y);
void sd_ClearBuffer(FrameBuffer_t *f, const Color_t c);
void sd_CopyTextureToBuffer(FrameBuffer_t *f, Image_t *i, 
 const int32_t x, const int32_t y); 
void sd_DrawLine(FrameBuffer_t *f, const int32_t ax, const int32_t ay,
 const int32_t bx, const int32_t by, const Color_t c);
void sd_DrawTriangle(FrameBuffer_t *f, const Tri_t t, const Color_t c);
void sd_DrawPixel(FrameBuffer_t *f, const int32_t x, const int32_t y, const Color_t c);
void sd_FreeFrameBuffer(FrameBuffer_t *f);

extern const Color_t SD_BLACK;
extern const Color_t SD_WHITE;
extern const Color_t SD_RED;
extern const Color_t SD_BLUE;
extern const Color_t SD_GREEN;

#endif
