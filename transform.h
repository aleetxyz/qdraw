#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "util/dll.h"
#include "mesh.h"
#include "vector.h"
#include <string.h>

#define MOV_TRF 1
#define SCA_TRF 2
#define ROT_TRF 3

typedef struct{
	char t;
	float x, y, z;
	float a;
}transform;

int new_transform(transform** p){
	transform* ttmp = (transform*)malloc(sizeof(transform));
	if(ttmp == NULL){
		perror("transform type memory allocation");
		return -1;
	}
	ttmp->t = 0;
	ttmp->x = 0.0;
	ttmp->y = 0.0;
	ttmp->z = 0.0;
	ttmp->a = 0.0;
	*p = ttmp;
	return 1;
}

int mklist_trf(char* fname, dl_list* list){
	FILE* fp;
	transform* t;	
	int br;
	if(!fname){
		perror("invalid file name");
		return -1;
	}	
	fp = fopen(fname, "r+");
	if(!(fp)){
		perror("fopen error");
		return -1;
	}
	dl_list trnf_list = NULL;
	while(1){
		new_transform(&t);
		br = fscanf(fp, "%c %f %f %f %f\n", &t->t, &t->x, &t->y, &t->z, &t->a);
		if(br > 0){
			dll_insertt(&trnf_list, t);
		}else{
			break;
		}
	}
	*list = trnf_list;
	return 0;
}

void traslate_f(face_t* face, float tx, float ty, float tz){
	vec_4df v0 = {face->v0->sv->x, face->v0->sv->y, face->v0->sv->z, 1.0};
	vec_4df v1 = {face->v1->sv->x, face->v1->sv->y, face->v1->sv->z, 1.0};
	vec_4df v2 = {face->v2->sv->x, face->v2->sv->y, face->v2->sv->z, 1.0};
	
	mat_4df M = {{1,0,0,tx},{0,1,0,ty},{0,0,1,tz},{0,0,0,1}};
	mat_4df_transpose(M);
	vec_4df r0, r1, r2;
	vec_mat_4df_mul(M, v0, r0);
	vec_mat_4df_mul(M, v1, r1);
	vec_mat_4df_mul(M, v2, r2);
	
	face->v0->sv->x = r0[0]; face->v0->sv->y = r0[1]; face->v0->sv->z = r0[2];
	face->v1->sv->x = r1[0]; face->v1->sv->y = r1[1]; face->v1->sv->z = r1[2];
	face->v2->sv->x = r2[0]; face->v2->sv->y = r2[1]; face->v2->sv->z = r2[2];
}

void scale_f(face_t* face, float dx, float dy, float dz){
	vec_4df v0 = {face->v0->sv->x, face->v0->sv->y, face->v0->sv->z, 1.0};
	vec_4df v1 = {face->v1->sv->x, face->v1->sv->y, face->v1->sv->z, 1.0};
	vec_4df v2 = {face->v2->sv->x, face->v2->sv->y, face->v2->sv->z, 1.0};
	
	mat_4df M = {{dx,0,0,0},{0,dy,0,0},{0,0,dz,0},{0,0,0,1}};
	mat_4df_transpose(M);
	vec_4df r0, r1, r2;
	vec_mat_4df_mul(M, v0, r0);
	vec_mat_4df_mul(M, v1, r1);
	vec_mat_4df_mul(M, v2, r2);
	
	face->v0->sv->x = v0[0]+r0[0]; face->v0->sv->y = v0[1]+r0[1]; face->v0->sv->z = v0[2]+r0[2];
	face->v1->sv->x = v1[0]+r1[0]; face->v1->sv->y = v1[1]+r1[1]; face->v1->sv->z = v1[2]+r1[2];
	face->v2->sv->x = v2[0]+r2[0]; face->v2->sv->y = v2[1]+r2[1]; face->v2->sv->z = v2[2]+r2[2];
}

void rotate_f(face_t* face, float x, float y, float z, float t){
	vec_4df v0 = {face->v0->sv->x, face->v0->sv->y, face->v0->sv->z, 1.0};
	vec_4df v1 = {face->v1->sv->x, face->v1->sv->y, face->v1->sv->z, 1.0};
	vec_4df v2 = {face->v2->sv->x, face->v2->sv->y, face->v2->sv->z, 1.0};
	
	float rd = (t * 3.1416)/180.0;
	float w = cos(rd/2);
	x *= sin(rd/2); y *= sin(rd/2); z *= sin(rd/2);	
	
	float n = 1.0/sqrt(x*x+y*y+z*z+w*w);
	x*=n; y*=n; z*=n; w*=n;
	
	mat_4df M = {
		{(w*w+x*x)-(y*y-z*z), (2*x*y)-(2*w*z), (2*x*z)+(2*w*y), 0.0},
		{(2*x*y)+(2*w*z), (w*w-x*x)-(y*y-z*z), (2*y*z)+(2*w*x), 0.0},
		{(2*x*z)-(2*w*y), (2*y*z)-(2*w*x), (w*w-x*x)-(y*y+z*z), 0.0},
		{0.0, 0.0, 0.0, 1.0}
	};
	mat_4df_transpose(M);
	vec_4df r0, r1, r2;
	vec_mat_4df_mul(M, v0, r0);
	vec_mat_4df_mul(M, v1, r1);
	vec_mat_4df_mul(M, v2, r2);
	
	face->v0->sv->x = r0[0]; face->v0->sv->y = r0[1]; face->v0->sv->z = r0[2];
	face->v1->sv->x = r1[0]; face->v1->sv->y = r1[1]; face->v1->sv->z = r1[2];
	face->v2->sv->x = r2[0]; face->v2->sv->y = r2[1]; face->v2->sv->z = r2[2];
}

void mk_trnf_f(face_t* face, transform* trf, float fps){
	switch(trf->t){
		case 'm':
			traslate_f(face, trf->x/fps, trf->y/fps, trf->z/fps);
			break;
		case 'r':
			rotate_f(face, trf->x/fps, trf->y/fps, trf->z, trf->a/fps);
			break;
		case 's':
			scale_f(face, trf->x/fps, trf->y/fps, trf->z/fps);
			break;
	}
}

#endif
