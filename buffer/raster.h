#ifndef RASTER_H
#define RASTER_H

#include <stdio.h>
#include "color.h"

#define PPM_VER	"P3"
#define PPM_PAL	255

typedef pixel** raster;

typedef struct{
	raster rst;
	res_t W, H;
}image;

void new_raster(int W, int H);
void del_raster();
//putpixel on img
void putpixel(int x, int y);
void putpixel_rgb(int x, int y, color* rgb);
//Bressenham
void draw_line(int x, int y, int x2, int y2);
void draw_line_rgb(int x, int y, int x2, int y2, color* rgb);
//draw io
int raster_ppm(FILE*);

//check bounds GLOBAL
int chk_clip_x(int x, int W){
	if(x < 0 || x >= W)
		return -1;
	else
		return 1;
}

int chk_clip_y(int y, int H){
	if(y < 0 || y >= H)
		return -1;
	else
		return 1;
}

int ccx(int x, int W){
	return chk_clip_x(x, W);
}

int ccy(int y, int H){
	return chk_clip_y(y, H);
}
#endif
