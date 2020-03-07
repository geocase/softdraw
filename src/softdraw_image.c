#include <stdio.h>
#include <stdlib.h>
#include "softdraw.h"
#include "softdraw_image.h"


// bitmap
struct bmpHeader {
	int8_t field[2];
	int32_t size;
	int32_t reserved;
	int32_t offset;
};

struct dibHeader {
	int32_t size;
	int32_t width;
	int32_t height;
	int16_t planes;
	int16_t bitsPerPixel;
	int32_t compression;
	int32_t imageSize; // 0 for uncompressed
	int32_t horizontalResolution;
	int32_t verticalResolution;
	int32_t colors;
	int32_t impColors;
};

enum dibHeaderSize {BITMAPINFOHEADER=40, BITMAPV4HEADER=108};

void sdiint_BuildBMPHeader(struct bmpHeader *h, FILE *f);
void sdiint_BuildDIBHeader(struct dibHeader *h, FILE *f);
void sdiint_FillDIBBitMapInfoHeader(struct dibHeader *h, FILE *f);


void sdiint_BuildBMPHeader(struct bmpHeader *h, FILE *f) {
	fseek(f, 0, SEEK_SET);
	// field
	fread(&(h->field), 1, 2, f);
	printf("field: %c%c\n", h->field[0], h->field[1]);

	// file size
	fread(&(h->size), 4, 1, f);
	printf("SIZE: %d\n", h->size);

	// reserved (junk data)
	fread(&(h->reserved), 4, 1, f);
	printf("junk: %d\n", h->reserved);
	
	// offset
	fread(&(h->offset), 4, 1, f);
	printf("offset: %d\n", h->offset);
}

void sdiint_BuildDIBHeader(struct dibHeader *h, FILE *f) {
	fseek(f, 14, SEEK_SET);
	// format type / size
	fread(&(h->size), 4, 1, f);
	printf("size: %d\n", h->size);
	
	switch(h->size) {
		case BITMAPINFOHEADER:
			sdiint_FillDIBBitMapInfoHeader(h, f);
			break;
	}
}

void sdiint_FillDIBBitMapInfoHeader(struct dibHeader *h, FILE *f) {
	fseek(f, 18, SEEK_SET);
	// coords
	fread(&(h->width), 4, 1, f);
	fread(&(h->height), 4, 1, f);
	printf("coords: %d, %d\n", h->width, h->height);

	// planes
	fread(&(h->planes), 2, 1, f);
	
	// BPP
	fread(&(h->bitsPerPixel), 2, 1, f);
	printf("BPP: %d\n", h->bitsPerPixel);

	// Compression method
	fread(&(h->compression), 4, 1, f);
	printf("comp: %d\n", h->compression);

	// image size
	fread(&(h->imageSize), 4, 1, f);
	printf("imageSize: %d\n", h->imageSize);

	// resolution
	fread(&(h->horizontalResolution), 4, 1, f);
	fread(&(h->verticalResolution), 4, 1, f);

	// colors
	fread(&(h->colors), 4, 1, f);	
	fread(&(h->impColors), 4, 1, f);	
}

Image_t *sdi_LoadBMP(char *path) {
	Image_t *t = malloc(sizeof(Image_t));

	printf("Loading BMP %s\n", path);

	FILE *f = fopen(path, "r");

	if(f) {
		printf("Loaded %s successfully\n", path);
	} else {
		printf("Loading %s failed\n", path);
		return NULL;
	}

	struct bmpHeader *bmpHead = malloc(sizeof(struct bmpHeader));
	struct dibHeader *dibHead = malloc(sizeof(struct dibHeader));
	sdiint_BuildBMPHeader(bmpHead, f);
	sdiint_BuildDIBHeader(dibHead, f);

	t->sizeX = dibHead->width;
	t->sizeY = dibHead->height;


	t->data = malloc(sizeof(Color_t) * t->sizeX * t->sizeY);
	Color_t *temp = malloc(sizeof(Color_t) * t->sizeX * t->sizeY);

	switch(dibHead->bitsPerPixel) {
		case 24:
			for(int32_t i = t->sizeX * t->sizeY; i >= 0 ; i--) {
				fread(&temp[i].b, 1, 1, f);
				fread(&temp[i].g, 1, 1, f);
				fread(&temp[i].r, 1, 1, f);
			}		
			break;
		default:
			break;
	}

	//Horizontal flip 
	for(int32_t x = t->sizeX; x >= 0; x--) {
		for(int32_t y = 0; y <= t->sizeY; y++) {
			t->data[t->sizeX * y + x] = temp[t->sizeX * y + (t->sizeX - x)];
		}
	}
	free(f);
	free(temp);

	return t;
}
