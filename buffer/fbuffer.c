#include "fbuffer.h"

fbuffer img_frm;

void new_fbuffer(int W, int H){
	int i, j;
	img_frm.frm = (color**)malloc(sizeof(color*)*H);
	for(i=0;i<H;i++){
		img_frm.frm[i] = (color*)malloc(sizeof(color)*W);
	}
	
	img_frm.H = H;
	img_frm.W = W;
	
	rst_fbuffer();
}

void del_fbuffer(){
	int i;
	for(i=0;i<img_frm.H;i++){
		free(img_frm.frm[i]);
	}
	free(img_frm.frm);
}

void rst_fbuffer(){
	int i, j;
	for(i=0;i<img_frm.H;i++){
		for(j=0;j<img_frm.W;j++){
			img_frm.frm[i][j].r = 0;
			img_frm.frm[i][j].g = 0;
			img_frm.frm[i][j].b = 0;
			img_frm.frm[i][j].a = 0;
		}
	}
}

void rst_fbufferc(int color){
	int i, j;
	for(i=0;i<img_frm.H;i++){
		for(j=0;j<img_frm.W;j++){
			img_frm.frm[i][j].r = color;
			img_frm.frm[i][j].g = color;
			img_frm.frm[i][j].b = color;
			img_frm.frm[i][j].a = 0;
		}
	}
}

int fbuffer_ppm(const char* filename){
	FILE* fp = fopen(filename, "w+");
	if(!fp)
		perror("fopen err");
	fprintf(fp, "%s\n%d %d\n%d\n", PPM_VER, img_frm.W, img_frm.H, PPM_PAL);
	int i, j;
	for(i=0;i<img_frm.H;i++){
		for(j=0;j<img_frm.W;j++){
			fprintf(fp,"%d %d %d\t", img_frm.frm[i][j].r, img_frm.frm[i][j].g, img_frm.frm[i][j].b);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}

void f_putpixel(int x, int y, color* rgb){
	if((chk_clip_x(x, img_frm.W) > 0) && (chk_clip_y(y, img_frm.H) > 0)){
		img_frm.frm[y][x].r = rgb->r;
		img_frm.frm[y][x].g = rgb->g;
		img_frm.frm[y][x].b = rgb->b;
	}
}
//debug purposes function
void f_ptpx(int x, int y, uchar r, uchar g, uchar b){
	if((chk_clip_x(x, img_frm.W) > 0) && (chk_clip_y(y, img_frm.H) > 0)){
		img_frm.frm[y][x].r = r;
		img_frm.frm[y][x].g = g;
		img_frm.frm[y][x].b = b;
	}
}

void dl(int x, int y, int x2, int y2) {
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
        f_ptpx(x,y,255,255,255);
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
