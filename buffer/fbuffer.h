#ifndef FBUFFER_H
#define FBUFFER_H

#include "raster.h"

typedef struct{
	color** frm;
	res_t W, H;
}fbuffer;

//initialize
void new_fbuffer(int W, int H);
void del_fbuffer();
void rst_fbuffer();
//putpixel
void f_putpixel(int x, int y, color* rgb);
//draw io
int fbuffer_ppm(const char*);

#endif 
