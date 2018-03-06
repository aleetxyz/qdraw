#ifndef SHADING_H
#define SHADING_H

#include "buffer/color.h"
#include "mesh.h"

#define O_AMBTL	0x1
#define O_POINT	0x2
#define O_SPOTL	0x4

#define O_FLATS	0x10
#define O_GOURD 0x20
#define	O_PHONG	0x40

#define MIN_RGB 0.05
#define MAX_RGB 1.0
#define O_FSRGB 255.0

//light sources

//ambient light
typedef struct{
	vec_4df pos;		//light position is at infinite
}ambnt_l;

//point light
typedef struct{		
	vec_3df pos;		//light position
	vec_3df att;		//attenuation
}point_l;

//spotlight
typedef struct{
	vec_3df pos;		//light position
	vec_3df att;		//attenuation
	vec_3df pnt; 		//light direction
	float 	cutoff;		//cone angle
}spotl_l;

//super type
typedef struct{
	ambnt_l* ambient;	//ambient light
	point_l* point;		//point light
	spotl_l* spot;		//spot light
	color_t* lint;	//light color
}light;

//material
typedef struct{
	color_t* color;		//surface color
	float ka;			//ambient coeff
	float kd;			//diffuse coeff
	float ks;			//speculr coeff
	int shns;			//shinniness cf
}material;
//constructors
/**********************************************************************************/
//generic constructor
int new_light(light** l){
	*l = (light*)malloc(sizeof(light));
	cptr(*l,"light");
	(*l)->ambient = NULL;
	(*l)->point = NULL;
	(*l)->spot = NULL;
	new_colorf(&(*l)->lint);
	return 0;
}
//ambient light constructor
int new_ambient(light* l){
	l->ambient = (ambnt_l*)malloc(sizeof(ambnt_l));
	cptr(l->ambient,"ambtl");
	return 0;
}
//point light constructor
int new_point(light* l){
	l->point = (point_l*)malloc(sizeof(point_l));
	cptr(l->point,"pntsl");
	return 0;
}
//spotlight constructor
int new_spot(light* l){
	l->spot = (spotl_l*)malloc(sizeof(spotl_l));
	cptr(l->spot,"spotl");
	return 0;
}
//material constructor
int new_material(material** m, float ka, float kd, float ks, int n){
	*m = (material*)malloc(sizeof(material));
	cptr(*m,"matrl");
	new_colorf(&(*m)->color);
	(*m)->ka = ka;
	(*m)->kd = kd;
	(*m)->ks = ks;
	(*m)->shns = n;
	return 0;
}
/**********************************************************************************/
//calc attenuation factor att = {cte,lin,quad}
float calc_fatt(vec_3df att, float d){	
	return min(1.0, 1.0/(att[0]+(att[1]*d)+(att[2]*d*d)));
}
//get halfway vector
void bp_halfway(vec_3df ld, vec_3df ftc, vec_3df H){
	add_3df(ftc, ld, H);
	nrm_3df(H);
}
//phong formula
void calc_phong(color_t* ia, color_t* id, color_t* is, float f, material* m, vec_3df r){
	r[0] = m->color->r * (ia->r + f * (id->r + is->r));
	r[1] = m->color->g * (ia->g + f * (id->g + is->g));
	r[2] = m->color->b * (ia->b + f * (id->b + is->b));
}
//check spotlight angle wrt light direction and face-to-light vector
int cone_angl(vec_3df lorg, vec_3df ldst, vec_3df stl, float ct){
	vec_3df ldir;
	sub_3df(ldst,lorg,ldir);
	nrm_3df(ldir);
	inv_3df(stl);
	float ltfa = dot_3df(stl, ldir);
	abs_3df(stl);
	//check if attenuaiton is affected
	if(ltfa > ct){
		return -1;
	}
	return 0;
}
//reflection calculations
/**********************************************************************************/
//ambient refelciton
void calc_ambr(color_t* ia, material* m, color_t* rc){
	//rc = ia * ka
	rc->r = ia->r * m->ka;
	rc->g = ia->g * m->ka;
	rc->b = ia->b * m->ka;
}
//diffuse reflection
void calc_difr(color_t* il, material* m, vec_3df fn, vec_3df ftl, color_t* rc){
	float dcf = dot_3df(fn, ftl);	//removed max(1,dcf)
	//rc = il * kd * N.L
	rc->r = il->r * m->kd * dcf;
	rc->g = il->g * m->kd * dcf;
	rc->b = il->b * m->kd * dcf;
}
//specular refelction using blinn-phong (light intensity, ks, face-nrm, face-to-cam vector, light direction)
void calc_spcr(color_t* il, material* m, vec_3df fn, vec_3df ftc, vec_3df ftl, color_t* rc){
	//use blinn-phong to get halfway vector
	vec_3df H;
	bp_halfway(ftl, ftc, H);
	nrm_3df(H);
	//rc = il * ks * (N.H)^n
	float scf = pow(dot_3df(fn, H), m->shns); //removed max(1,scf)
	rc->r = il->r * m->ks * scf;
	rc->g = il->g * m->ks * scf;
	rc->b = il->b * m->ks * scf;
}
/**********************************************************************************/
//calculate light effect at a position
void calc_light(light* l, vec_3df fvp, vec_3df fn, vec_3df ftc, material* m, vec_3df r){
	color_t *a,*d,*s;
	vec_3df op = {fvp[0],fvp[1],fvp[2]};	//object position
	vec_3df ftl;							//face-to-ilght v
	float f;
	new_colorf(&a);
	new_colorf(&d);
	new_colorf(&s);
	//calc ambient reflection, the same 4 any light type
	calc_ambr(l->lint, m, a);
	//calculate diffuse/spec reflection for an ambient light
	if(l->ambient != NULL){
		//no attenuation for ambient light
		f = 0.0;
		nrm_3df(l->ambient->pos);
		//calculate diffuse refelction
		calc_difr(l->lint, m, fn, op, d);
		//calculate specular refelciton
		calc_spcr(l->lint, m, fn, ftc, op, s);
	}
	if(l->point != NULL){
		sub_3df(l->point->pos, op, ftl);
		nrm_3df(ftl);
		nrm_3df(ftc);
		//calc pointlight attenuation
		f = calc_fatt(l->point->att, len_3df(ftl));
		//calc diffuse reflection
		calc_difr(l->lint, m, fn, ftl, d);
		//calc specular reflection
		calc_spcr(l->lint, m, fn, ftc, ftl, s);
	}
	if(l->spot != NULL){
		sub_3df(l->spot->pos, op, ftl);
		nrm_3df(ftl);
		nrm_3df(ftc);
		//calc spotlight attenuation
		f = calc_fatt(l->spot->att, len_3df(ftl));
		//cone angle restrictions
		if(cone_angl(l->spot->pos, l->spot->pnt, ftl, l->spot->cutoff) < 0){
			f = 0.0;
		}
		//calc diffuse reflection
		calc_difr(l->lint, m, fn, ftl, d);
		//calc specular reflection
		calc_spcr(l->lint, m, fn, ftc, ftl, s);
	}
	//apply phong model
	calc_phong(a, d, s, f, m, r);
	//clamp-to-range vec3f color
	ctr_3df(r, MIN_RGB, MAX_RGB);
}

/********************************SHADING**************************************/
/********************************SHADING**************************************/

//calculate intensity by face (flat)
void calc_facei(light* light, face_t* face, material* m){
	vec_3df p; set_vec_3df(p,face->bc->x, face->bc->y, face->bc->z);
	
	calc_light(light, p, face->fn, face->fc, m, face->fi);
}
//calculate intensity by vertex (gouraud)
void calc_vrtxi(light* light, face_t* face, material* m){
	vec_3df p0; set_vec_3df(p0, face->v0->sv->x, face->v0->sv->y, face->v0->sv->z);
	vec_3df p1; set_vec_3df(p1, face->v1->sv->x, face->v1->sv->y, face->v1->sv->z);
	vec_3df p2; set_vec_3df(p2, face->v2->sv->x, face->v2->sv->y, face->v2->sv->z);
	
	calc_light(light, p0, face->fn, face->fc, m, face->v0->vi);
	calc_light(light, p1, face->fn, face->fc, m, face->v1->vi);
	calc_light(light, p2, face->fn, face->fc, m, face->v2->vi);
}
//calculate intensity by pixel (phong)
void calc_pixli(light* light, face_t* face, material* m, vec_3df n, vec_3df rc){
	vec_3df p; set_vec_3df(p,face->bc->x, face->bc->y, face->bc->z);	
	calc_light(light, p, n, face->fc, m, rc);
}

/********************************math stuff*****************************************/
//barycentric interpolation (area, weights, color)
void bb_interpolate(vec_3df w, vec_3df v0, vec_3df v1, vec_3df v2, vec_3df rc){	
	rc[0] = w[0] * v0[0] + w[1] * v1[0] + w[2] * v2[0];
	rc[1] = w[0] * v0[1] + w[1] * v1[1] + w[2] * v2[1];
	rc[2] = w[0] * v0[2] + w[1] * v1[2] + w[2] * v2[2];
}
/**********************************************************************************/
//shading function (face, light, material, barycoord, option, result color)
void fshade(face_t* face, light* l, material* m, vec_3df w, flag mode, color* r){
	
	vec_3df rc, n;
	
	//flat shading face-to-cam, face-normal based
	if(mode & O_FLATS){
		memcpy(rc, face->fi, sizeof(face->fi));
		nrm_3df(face->fc);
		smul_3df(rc, dot_3df(face->fn, face->fc));
	}
	//gouraud shading, barycentric-bounding-box algo based
	if(mode & O_GOURD){
		bb_interpolate(w, face->v0->vi, face->v1->vi, face->v2->vi, rc);
	}
	//phong shading, barycentric-bounding-box algo based
	if(mode & O_PHONG){
		bb_interpolate(w, face->v0->vn, face->v1->vn, face->v2->vn, n);
		nrm_3df(n);	//nomralize interpolation result
		calc_pixli(l, face, m, n, rc);	//apply light
	}
	
	smul_3df(rc, O_FSRGB);
	vec3f_to_rgb(r, rc);	
}
//apply illumination model before RASTER STAGE (FLAT AND GOURAUD)
void apply_meshi(dl_list face_list, light *l, material* m){
	list_it it = face_list;
	face_t* ftmp;
	while(hasNext(it)){
		ftmp = (face_t*)it->data;
		calc_facei(l, ftmp, m);
		calc_vrtxi(l, ftmp, m);
		it = it->next;
	}	

}
#endif
