#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "softdraw.h"
#include "softdraw_image.h"

#define WINDOW_X 320 
#define WINDOW_Y 240

uint8_t running = 1;

int mouseX = 0, mouseY = 0;

void m_InitSDL();
void m_BlitPixels(FrameBuffer_t *t); 
void m_Input();

SDL_Window *sdlWin;
SDL_Renderer *sdlRen;

int main() {
	srand(time(NULL));

	m_InitSDL();
	FrameBuffer_t *buf = sd_NewFrameBuffer(WINDOW_X, WINDOW_Y);	

	Tri_t k = {.v0[0] = 80, .v0[1] = 150,
	           .v1[0] = mouseX, .v1[1] = mouseY,
	           .v2[0] = 240, .v2[1] = 150};
	
	while(running) {
		k.v1[0] = mouseX;
		k.v1[1] = mouseY;
		sd_DrawTriangle(buf, k, SD_WHITE);
		m_BlitPixels(buf);
		sd_ClearBuffer(buf, SD_BLACK);
		m_Input();
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

void m_Input() {
	SDL_PumpEvents();
	SDL_GetMouseState(&mouseX, &mouseY);

	// Window
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT) {
			running = 0;
		}
	}
}
