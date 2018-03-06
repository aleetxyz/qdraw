#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "raster.h"

typedef struct{
	uchar grs;
	int z_crd;
}z_t;

typedef struct{
	z_t** zbuf;
	res_t W, H;
}zbuffer;

//initialize
void new_zbuffer(int W, int H, int dp);
void del_zbuffer();
//putpixel
void z_putpixel(int x, int y);
//draw io
int zbuffer_ppm(const char*);
///check zcoord vs depth
int z_compare(int i, int j, int z);

#endif

