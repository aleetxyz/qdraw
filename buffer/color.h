#ifndef COLOR_H
#define COLOR_H

#include <stdlib.h>
#include <stdint.h>

//imagery definitions
#define FHD_WID	1920
#define FHD_HEI	1080
#define SXG_WID 1280
#define SXG_HEI 1024
#define MHD_WID 1280
#define MHD_HEI 720
#define XGA_WID 1024
#define XGA_HEI 768
#define SVG_WID	800
#define SVG_HEI	600
#define VGA_WID 640
#define VGA_HEI 480

typedef unsigned char uchar;
typedef unsigned short res_t;

typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
}color;

typedef struct{
	float r;
	float g;
	float b;
	float a;
}color_t;

typedef struct{
	uchar r;
	uchar g;
	uchar b;
}pixel;

int new_color(color** rgb){
	*rgb = (color*)malloc(sizeof(color));
	if(rgb == NULL){
		perror("color memory allocation");
		return -1;
	}
	(*rgb)->r = (*rgb)->g = (*rgb)->g = (*rgb)->a = 0;
	return 0;
}

int new_colorf(color_t** rgb){
	*rgb = (color_t*)malloc(sizeof(color_t));
	if(rgb == NULL){
		perror("color memory allocation");
		return -1;
	}
	(*rgb)->r = (*rgb)->g = (*rgb)->g = (*rgb)->a = 0.0;
	return 0;
}

void vec3f_to_rgb(color* rgb, float* vec3f){
	rgb->r = (int)(vec3f[0]);
	rgb->g = (int)(vec3f[1]);
	rgb->b = (int)(vec3f[2]);
	rgb->a = 0.0;
}

#endif
