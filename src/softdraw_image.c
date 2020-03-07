#include <stdio.h>
#include <stdlib.h>
#include "softdraw.h"
#include "softdraw_image.h"

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

	// BM CHECK
	int8_t buffer[2];
	fread(&buffer, 1, 2, f);
	if(buffer[0] == 'B' && buffer[1] == 'M') {
		printf("File %s is a bitmap file\n", path);
	} else {
		printf("File %s is NOT a bitmap file\n", path);
		return NULL;
	}

	int32_t size;
	fread(&size, 4, 1, f);
	printf("SIZE: %d\n", size);
	fseek(f, 4, SEEK_CUR);
	
	int32_t dataOffset;
	fread(&dataOffset, 4, 1, f);
	printf("dataOffset: %d\n", dataOffset);

	int32_t infoHeader;
	fread(&infoHeader, 4, 1, f);
	printf("infoHeader: %d\n", infoHeader);

	fread(&t->sizeX, 4, 1, f);
	fread(&t->sizeY, 4, 1, f);
	printf("x %d, y %d\n", t->sizeX, t->sizeY);

	t->data = malloc(sizeof(Color_t) * t->sizeX * t->sizeY);
	
	fseek(f, 2, SEEK_CUR);

	int16_t bitspp;
	fread(&bitspp, 2, 1, f);
	printf("bitspp %d\n", bitspp);

	int32_t compression;
	fread(&compression, 5, 1, f);
	printf("comp %d\n", compression);

	fseek(f, dataOffset, SEEK_SET);

	Color_t *temp = malloc(sizeof(Color_t) * t->sizeX * t->sizeY);

	// Color table
	for(int32_t i = t->sizeX * t->sizeY; i >= 0 ; i--) {
		fread(&temp[i].b, 1, 1, f);
		fread(&temp[i].g, 1, 1, f);
		fread(&temp[i].r, 1, 1, f);
	}

	//Horizontal adjust
	for(int32_t x = t->sizeX; x >= 0; x--) {
		for(int32_t y = 0; y <= t->sizeY; y++) {
			t->data[t->sizeX * y + x] = temp[t->sizeX * y + (t->sizeX - x)];
		}
	}

	printf("0 value:%d, %d, %d", t->data[0].r, t->data[0].g, t->data[0].b);

	return t;
}
