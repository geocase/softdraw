#include <stdlib.h>
#include "softdraw.h"

const Color_t SD_BLACK = {0, 0, 0};
const Color_t SD_WHITE = {255, 255, 255};
const Color_t SD_RED = {255, 0, 0};
const Color_t SD_BLUE = {0, 255, 0};
const Color_t SD_GREEN = {0, 0, 255};

void sdint_DrawLineBresenham(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c);

void sdint_DrawLineNaive(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c);

FrameBuffer_t *sd_NewFrameBuffer(const uint32_t x, const uint32_t y) {
	FrameBuffer_t *tBuff = malloc(sizeof(FrameBuffer_t));
	tBuff->buffer = malloc(sizeof(Color_t) * x * y);
	tBuff->sizeX = x;
	tBuff->sizeY = y;

	sd_ClearBuffer(tBuff, SD_BLACK);	

	return tBuff;
}

void sd_ClearBuffer(FrameBuffer_t *f, const Color_t c) {
	for(int nx = 0; nx < f->sizeX; nx++) {
		for(int ny = 0; ny < f->sizeY; ny++) {
			sd_DrawPixel(f, nx, ny, c);
		}
	}
}

void sd_DrawPixel(FrameBuffer_t *f, const uint32_t x, const uint32_t y, const Color_t c) {
	f->buffer[f->sizeX * y + x] = c;
}

void sdint_DrawLineBresenham(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c) {
	int32_t deltaX = abs(bx - ax);
	int8_t signX = ax < bx ? 1 : -1;
	int32_t deltaY = abs(by - ay);
	int8_t signY = ay < by ? 1 : -1;
	
	double err = (deltaX > deltaY ? deltaX : -deltaY) / 2, e2;

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

void sdint_DrawLineNaive(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c) {
	uint32_t dx = abs(bx - ax);
	uint32_t dy = abs(by - ay);
	uint32_t y;

	for(uint32_t x = ax; x <= bx; x++) {
		y = ay + dy * (x - ax) / dx;
		sd_DrawPixel(f, x, y, c);
	}		

}

void sd_DrawLine(FrameBuffer_t *f, const uint32_t ax, const uint32_t ay,
 const uint32_t bx, const uint32_t by, const Color_t c) {
	sdint_DrawLineBresenham(f, ax, ay, bx, by, c);
}

void sd_FreeFrameBuffer(FrameBuffer_t *f) {
	free(f->buffer);
	free(f);
}
