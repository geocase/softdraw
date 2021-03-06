#include <stdlib.h>
#include "softdraw.h"
#include "softdraw_image.h"

const Color_t SD_BLACK = {0, 0, 0};
const Color_t SD_WHITE = {255, 255, 255};
const Color_t SD_RED = {255, 0, 0};
const Color_t SD_BLUE = {0, 0, 255};
const Color_t SD_GREEN = {0, 255, 0};

FrameBuffer_t*
sd_NewFrameBuffer(const int32_t x, const int32_t y) {
	if(x < 0 || y < 0) return NULL;

	FrameBuffer_t *tBuff = malloc(sizeof(FrameBuffer_t));
	tBuff->buffer = malloc(sizeof(Color_t) * x * y);
	tBuff->sizeX = x;
	tBuff->sizeY = y;

	sd_ClearBuffer(tBuff, SD_BLACK);	

	return tBuff;
}

void
sd_ClearBuffer(FrameBuffer_t *f, const Color_t c) {
	for(int nx = 0; nx < f->sizeX; nx++) {
		for(int ny = 0; ny < f->sizeY; ny++) {
			sd_DrawPixel(f, nx, ny, c);
		}
	}
}

void
sd_CopyTextureToBuffer(FrameBuffer_t *f, Image_t *i, 
 const int32_t x, const int32_t y) {
	for(int nx = 0; nx < i->sizeX; nx++) {
		for(int ny = 0; ny < i->sizeY; ny++) {
			f->buffer[f->sizeX * (ny + y) + (nx + x)] = i->data[i->sizeX * ny + nx];
		}
	}
}

void
sd_DrawPixel(FrameBuffer_t *f, const int32_t x, const int32_t y, 
 const Color_t c) {
	if(x < 0 || y < 0) return;
	f->buffer[f->sizeX * y + x] = c;
}

void 
sd_DrawLine(FrameBuffer_t *f, const int32_t ax, const int32_t ay,
 const int32_t bx, const int32_t by, const Color_t c) {
	int32_t deltaX = abs(bx - ax);
	int8_t signX = ax < bx ? 1 : -1;
	int32_t deltaY = abs(by - ay);
	int8_t signY = ay < by ? 1 : -1;
	
	double err = (deltaX > deltaY ? deltaX : -deltaY) / 2;
	double e2;

	uint32_t y = ay;
	uint32_t x = ax;

	while(1) {
		sd_DrawPixel(f, x, y, c);
		if(x == bx && y == by) break;
		e2 = err;
	
		if(e2 > -deltaX) {
			err -= deltaY; 
			x += signX;
		}	
		if(e2 < deltaY) {
			err += deltaX;
			y += signY;
		}
	}
	
}

void
sd_DrawTriangle(FrameBuffer_t *f, const Tri_t t, const Color_t c) {
	sd_DrawLine(f, t.v0[0], t.v0[1], t.v1[0], t.v1[1], c);
	sd_DrawLine(f, t.v1[0], t.v1[1], t.v2[0], t.v2[1], c);
	sd_DrawLine(f, t.v2[0], t.v2[1], t.v0[0], t.v0[1], c);
}

void
sd_FreeFrameBuffer(FrameBuffer_t *f) {
	free(f->buffer);
	free(f);
}
