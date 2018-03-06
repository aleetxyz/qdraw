#ifndef MESH_H
#define MESH_H

#include "util/dll.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	float x, y;
}p2df;

typedef struct{
	float x, y, z;
}p3df;

typedef struct{
	int i;
	p3df*	sv;		//space coords
	p3df*	cc;		//camera coords
	p2df*	pv;		//screen coords
	vec_3df	vn;		//vertx normal
	vec_3df	vi;		//vertx intensity
	int 	z;		//z coords
}vrtx_t;

typedef struct{
	int i;
	vrtx_t *v0, *v1, *v2;	//vertexes
	p3df* 	bc;				//barycenter
	vec_3df fc;				//face to cam
	vec_3df fn;				//face normal
	vec_3df fi;				//face intensity
}face_t;

int new_p2df(p2df** p){
	p2df* ptmp = (p2df*)malloc(sizeof(p2df));
	cptr(ptmp,"p2df");
	ptmp->x = ptmp->y = 0.0f;
	*p = ptmp;
	return 1;
}

int new_p3df(p3df** p){
	p3df* ptmp = (p3df*)malloc(sizeof(p3df));
	cptr(ptmp,"p3df");
	ptmp->x = ptmp->y = ptmp->z = 0.0f;
	*p = ptmp;
	return 1;
}

int new_vrtx(vrtx_t** vrtx){
	vrtx_t* vtmp = (vrtx_t*)malloc(sizeof(vrtx_t));
	cptr(vtmp,"vertex");
	p3df* sv; new_p3df(&sv);
	p3df* cc; new_p3df(&cc);
	p2df* pv; new_p2df(&pv);
	vtmp->sv = sv; vtmp->cc = cc; vtmp->pv = pv;
	vtmp->vn[0] = vtmp->vn[1] = vtmp->vn[2] = 0.0f;
	vtmp->vi[0] = vtmp->vi[1] = vtmp->vi[2] = 0.0f;
	vtmp->z = vtmp->i = 0;
	*vrtx = vtmp;
	return 1;
}

int new_face(face_t** face){
	face_t* ftmp = (face_t*)malloc(sizeof(face_t));
	cptr(ftmp,"face");
	//vertex alloc
	vrtx_t *v0, *v1, *v2;
	new_vrtx(&v0); new_vrtx(&v1); new_vrtx(&v2);
	ftmp->v0 = v0; ftmp->v1 = v1; ftmp->v2 = v2;
	//barycenter
	p3df* bc; new_p3df(&bc); ftmp->bc = bc;
	//face-to-cam, face-normal
	ftmp->i = 0;
	ftmp->fc[0] = ftmp->fc[1] = ftmp->fc[2] = 0.0f;
	ftmp->fn[0] = ftmp->fn[1] = ftmp->fn[2] = 0.0f;
	ftmp->fi[0] = ftmp->fi[1] = ftmp->fi[2] = 0.0f;
	*face = ftmp;
	return 1;
}

void set_p2df(p2df* pt, float x, float y){
	pt->x = x;
	pt->y = y;
}

void set_p3df(p3df* pt, float x, float y, float z){
	pt->x = x;
	pt->y = y;
	pt->z = z;
}

void p2df_tostring(void* p){
	nptr(p);
	p2df* pt = (p2df*)p;
	printf("2d: x: %f, y: %f\n", pt->x, pt->y);
}

void p3df_tostring(void* p){
	nptr(p);
	p3df* pt = (p3df*)p;
	printf("3d: x: %f, y: %f, z: %f\n", pt->x, pt->y, pt->z);
}

void vrtx_tostring(void* v){
	nptr(v);
	vrtx_t* vt = (vrtx_t*)v;
	p2df_tostring(vt->pv);
	p3df_tostring(vt->sv);
	printf("vi:%d vn: %fi %fj %fk\nZ:%f\n", vt->i, vt->vn[0], vt->vn[1], vt->vn[2], vt->z);
}

void face_tostring(void* f){
	nptr(f);
	face_t* ft = (face_t*)f;
	vrtx_tostring(ft->v0);
	vrtx_tostring(ft->v1);
	vrtx_tostring(ft->v2);
	printf("fi:%d fn: %fi %fj %fk\n", ft->i, ft->fn[0], ft->fn[1], ft->fn[2]);	
}

int p3df_eql(p3df* a, p3df* b){
	norp(a,b);
	if(a->x == b->x && a->y == b->y && a->z == b->z){
		return 1;
	}else{
		return -1;
	}
}

int vrtx_eql(vrtx_t* a, vrtx_t* b){
	norp(a,b);
	if(p3df_eql(a->sv, b->sv) > 0){
		return 1;
	}else{
		return -1;
	}
}

int face_cont_vrtx(face_t* f, vrtx_t* v){
	norp(f, v);
	if(vrtx_eql(f->v0, v) == 1 || vrtx_eql(f->v1, v) == 1 || vrtx_eql(f->v2, v) == 1){
		return 1;
	}else{
		return -1;
	}	
}
//get a face centroid
void face_get_cnt(face_t* f){
	nptr(f)
	f->bc->x = (f->v0->sv->x + f->v1->sv->x + f->v2->sv->x)/3.0;
	f->bc->y = (f->v0->sv->y + f->v1->sv->y + f->v2->sv->y)/3.0;
	f->bc->z = (f->v0->sv->z + f->v1->sv->z + f->v2->sv->z)/3.0;
}
//get a face-to-cam vector 
void face_get_ftc(vec_3df fbc, vec_3df cpv, vec_3df r){
	sub_3df(fbc,cpv,r);
}
//get a face normal
void face_get_nrm(face_t* f){
	nptr(f);
	vec_3df a, b, c, va, vb, fn;
	a[0] = f->v0->sv->x, b[0] = f->v1->sv->x, c[0] = f->v2->sv->x;
	a[1] = f->v0->sv->y, b[1] = f->v1->sv->y, c[1] = f->v2->sv->y;
	a[2] = f->v0->sv->z, b[2] = f->v1->sv->z, c[2] = f->v2->sv->z;
	sub_3df(a, b, va);
	sub_3df(c, b, vb);
	crs_3df(vb, va, fn);
	nrm_3df(fn);
	memcpy(&f->fn, fn, sizeof(fn));
}
//get a vertex normal
void vrtx_get_nrm(dl_list vlf_list, vrtx_t* v){
	norp(vlf_list, v);
	list_it it = vlf_list;
	face_t* ftmp;
	vec_3df vn;
	set_vec_3df(vn, 0, 0, 0);
	while(hasNext(it)){
		ftmp = (face_t*)it->data;
		add_3df(ftmp->fn, vn, vn);
		it = it->next;
	}
	nrm_3df(vn);
	memcpy(&v->vn, vn, sizeof(vn));
}
//get inverse f z coord
float z_inverse(float z){
	return 1.0/z;
}
//pineda's edge function
float p_edgef(float* a, float* b, float* p){
	return (p[0]-a[0])*(b[1]-a[1])-(p[1]-a[1])*(b[0]-a[0]);
}
///
int new_vrtxi(vrtx_t** vrtx, int i){
	vrtx_t* vtmp = (vrtx_t*)malloc(sizeof(vrtx_t));
	cptr(vtmp,"vertex");
	p3df* sv; new_p3df(&sv);
	p3df* cc; new_p3df(&cc);
	p2df* pv; new_p2df(&pv);
	vtmp->sv = sv; vtmp->cc = cc; vtmp->pv = pv;
	vtmp->i = i;
	vtmp->vn[0] = vtmp->vn[1] = vtmp->vn[2] = 0.0f;
	vtmp->vi[0] = vtmp->vi[1] = vtmp->vi[2] = 0.0f;
	vtmp->z = 0;
	*vrtx = vtmp;
	return 1;
}

int new_facei(face_t** face, int i){
	face_t* ftmp = (face_t*)malloc(sizeof(face_t));
	cptr(ftmp,"face");
	//vertex alloc
	vrtx_t *v0, *v1, *v2;
	new_vrtx(&v0); new_vrtx(&v1); new_vrtx(&v2);
	ftmp->v0 = v0; ftmp->v1 = v1; ftmp->v2 = v2;
	//barycenter
	p3df* bc; new_p3df(&bc); ftmp->bc = bc;
	//face-to-cam, face-normal
	ftmp->i = i;
	ftmp->fc[0] = ftmp->fc[1] = ftmp->fc[2] = 0.0f;
	ftmp->fn[0] = ftmp->fn[1] = ftmp->fn[2] = 0.0f;
	*face = ftmp;
	return 1;
}
#endif
