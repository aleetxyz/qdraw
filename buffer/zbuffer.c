#include "zbuffer.h"

zbuffer img_zbuf;

void new_zbuffer(int W, int H, int z){
	int i, j;
	img_zbuf.zbuf = (z_t**)malloc(sizeof(z_t)*H);
	for(i=0;i<H;i++){
		img_zbuf.zbuf[i] = (z_t*)malloc(sizeof(z_t)*W);
	}
	
	for(i=0;i<H;i++){
		for(j=0;j<W;j++){
			img_zbuf.zbuf[i][j].grs = 0;
			img_zbuf.zbuf[i][j].z_crd = z;
		}
	}
	
	img_zbuf.H = H;
	img_zbuf.W = W;
}

void del_zbuffer(){
	int i;
	for(i=0;i<img_zbuf.H;i++){
		free(img_zbuf.zbuf[i]);
	}
	free(img_zbuf.zbuf);
}

void rst_zbuffer(int z){
	int i, j;
	for(i=0;i<img_zbuf.H;i++){
		for(j=0;j<img_zbuf.W;j++){
			img_zbuf.zbuf[i][j].grs = 0;
			img_zbuf.zbuf[i][j].z_crd = z;
		}
	}
}

void z_putpixel(int x, int y){
	if((chk_clip_x(x, img_zbuf.W) > 0) && (chk_clip_y(y, img_zbuf.H) > 0)){
		img_zbuf.zbuf[y][x].grs = (int)(255 - img_zbuf.zbuf[y][x].z_crd) % 255;
	}
}

int zbuffer_ppm(const char* filename){
	FILE* fp = fopen(filename, "w+");
	if(!fp)
		perror("fopen err");
	fprintf(fp, "%s\n%d %d\n%d\n", PPM_VER, img_zbuf.W, img_zbuf.H, PPM_PAL);
	int i, j;
	for(i=0;i<img_zbuf.H;i++){
		for(j=0;j<img_zbuf.W;j++){
			fprintf(fp,"%d %d %d\t", img_zbuf.zbuf[i][j].grs, img_zbuf.zbuf[i][j].grs, img_zbuf.zbuf[i][j].grs);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}

void z_interpolate(float p0, float p1, float p2, float* pt, float* z){
	*z = p0*pt[0] + p1*pt[1] + p2*pt[2];
	*z = 1/(*z);
}

int z_compare(int i, int j, int z){
	if(z < img_zbuf.zbuf[j][i].z_crd){
		return 1;
	}else{
		return 0;
	}	
}

void z_setcoord(int i, int j, int z){
	img_zbuf.zbuf[j][i].z_crd = z;
}

