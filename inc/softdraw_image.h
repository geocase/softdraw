#ifndef SOFTDRAW_IMAGE_H
#define SOFTDRAW_IMAGE_H
#include "softdraw.h"
typedef struct {
	int32_t sizeX, sizeY;
	Color_t *data;
} Image_t;

Image_t *sdi_LoadBMP(char *path);
#endif
