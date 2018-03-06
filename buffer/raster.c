#include "raster.h"

image img_rst;

void new_raster(int W, int H){
	int i, j;
	img_rst.rst = (raster)malloc(sizeof(pixel*)*H);
	for(i=0;i<H;i++){
		img_rst.rst[i] = (pixel*)malloc(sizeof(pixel)*W);
	}
	
	for(i=0;i<H;i++){
		for(j=0;j<W;j++){
			img_rst.rst[i][j].r = 0;
			img_rst.rst[i][j].g = 0;
			img_rst.rst[i][j].b = 0;
		}
	}
	
	img_rst.H = H;
	img_rst.W = W;
}

void del_raster(){
	int i;
	for(i=0;i<img_rst.H;i++){
		free(img_rst.rst[i]);
	}
	free(img_rst.rst);
}

void rst_raster(){
	int i, j;
	for(i=0;i<img_rst.H;i++){
		for(j=0;j<img_rst.W;j++){
			img_rst.rst[i][j].r = 0;
			img_rst.rst[i][j].g = 0;
			img_rst.rst[i][j].b = 0;
		}
	}
}

int raster_ppm(FILE* fp){
	fprintf(fp, "%s\n%d %d\n%d\n", PPM_VER, img_rst.W, img_rst.H, PPM_PAL);
	int i, j;
	for(i=0;i<img_rst.H;i++){
		for(j=0;j<img_rst.W;j++){
			fprintf(fp,"%d %d %d\t", img_rst.rst[i][j].r, img_rst.rst[i][j].g, img_rst.rst[i][j].b);
		}
		fprintf(fp, "\n");
	}
	return 0;
}

void putpixel(int x, int y){
	if((chk_clip_x(x, img_rst.W) > 0) && (chk_clip_y(y, img_rst.H) > 0)){
		img_rst.rst[y][x].r = 255;
		img_rst.rst[y][x].g = 255;
		img_rst.rst[y][x].b = 255;
	}
}

void putpixel_rgb(int x, int y, color* rgb){
	if((chk_clip_x(x, img_rst.W) > 0) && (chk_clip_y(y, img_rst.H) > 0)){
		img_rst.rst[y][x].r = rgb->r;
		img_rst.rst[y][x].g = rgb->g;
		img_rst.rst[y][x].b = rgb->b;
	}
}

void ptpx(int x, int y, uchar r, uchar g, uchar b){
	if((chk_clip_x(x, img_rst.W) > 0) && (chk_clip_y(y, img_rst.H) > 0)){
		img_rst.rst[y][x].r = r;
		img_rst.rst[y][x].g = g;
		img_rst.rst[y][x].b = b;
	}
}

void ptln(int x, int y, int x2, int y2, uchar r, uchar g, uchar b){
	color* rgb = (color*)malloc(sizeof(color));
	rgb->r = r;
	rgb->g = g;
	rgb->b = b;
	rgb->a = 0;
	draw_line_rgb(x, y, x2, y2, rgb);
}

void draw_line(int x, int y, int x2, int y2){
	color* rgb = (color*)malloc(sizeof(color));
	rgb->r = 255;
	rgb->g = 255;
	rgb->b = 255;
	rgb->a = 0;
	draw_line_rgb(x, y, x2, y2, rgb);
}

void draw_line_rgb(int x, int y, int x2, int y2, color* rgb) {
    int w = x2 - x ;
    int h = y2 - y ;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0, i;
    if (w<0) dx1 = -1 ; else if (w>0) dx1 = 1 ;
    if (h<0) dy1 = -1 ; else if (h>0) dy1 = 1 ;
    if (w<0) dx2 = -1 ; else if (w>0) dx2 = 1 ;
    int longest = abs(w) ;
    int shortest = abs(h) ;
    if (!(longest>shortest)) {
        longest = abs(h) ;
        shortest = abs(w) ;
        if (h<0) 
			dy2 = -1; 
		else if (h>0) 
			dy2 = 1;
		dx2 = 0;            
    }
    int numerator = longest >> 1 ;
    for (i=0;i<=longest;i++) {
        putpixel_rgb(x,y,rgb);
        numerator += shortest ;
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } else {
            x += dx2 ;
            y += dy2 ;
        }
    }
}


