#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "softdraw.h"

#define WINDOW_X 640
#define WINDOW_Y 480

void m_InitSDL();
void m_BlitPixels(FrameBuffer_t *t); 

SDL_Window *sdlWin;
SDL_Renderer *sdlRen;

int main() {
	srand(time(NULL));

	m_InitSDL();
	FrameBuffer_t *buf = sd_NewFrameBuffer(WINDOW_X, WINDOW_Y);	
	
	Color_t col = {0, 0, 0};

	for(int i = 0; i < 1000; i++) {
		col.r = (i % 255);
		col.g = (i % 255);
		col.b = (i % 255);
		sd_ClearBuffer(buf, col);
		m_BlitPixels(buf);
	}

	sd_FreeFrameBuffer(buf);

	return 0;
}

void m_InitSDL() {
	sdlWin = SDL_CreateWindow("Softdraw Demo", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_X, WINDOW_Y, SDL_WINDOW_SHOWN);
	sdlRen = SDL_CreateRenderer(sdlWin, -1, SDL_RENDERER_ACCELERATED);
}

void m_BlitPixels(FrameBuffer_t *t) {
	for(int x = 0; x < t->sizeX; x++) {
		for(int y = 0; y < t->sizeY; y++) {
			Color_t c = t->buffer[t->sizeX * y + x];
			SDL_SetRenderDrawColor(sdlRen, c.r, c.g, c.b, 255);
			SDL_RenderDrawPoint(sdlRen, x, y);
		}
	}
	SDL_RenderPresent(sdlRen);
}
