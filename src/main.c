#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "softdraw.h"
#include "softdraw_image.h"

#define WINDOW_X 1000 
#define WINDOW_Y 500

uint8_t running = 1;

int mouseX = 0, mouseY = 0;

void m_InitSDL();
void m_BlitPixels(Image_t *t); 
void m_Input();

SDL_Window *sdlWin;
SDL_Renderer *sdlRen;

int main() {
	m_InitSDL();

	Image_t *i = sdi_LoadBMP("sample2.bmp");


	FrameBuffer_t *buf = sd_NewFrameBuffer(WINDOW_X, WINDOW_Y);	

	while(running) {
		m_BlitPixels(i);
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

void m_BlitPixels(Image_t *t) {
	for(int x = 0; x < t->sizeX; x++) {
		for(int y = 0; y < t->sizeY; y++) {
			Color_t c = t->data[t->sizeX * y + x];
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
