#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "mesh.h"
#include <string.h>

//UTIL CONSTR
#define O_PROJT 0x5
#define O_FRUST 0x6
#define O_ORTHO 0x7

#define CAM_NCP	1.00
#define CAM_FCP	1000

typedef struct{
	mat_4df cam, mpr;	//camera coords / matrix projection
	float 	ncp;		//near clip plane
	float 	fcp;		//far clip plane
	float 	aov;		//angle of vision
	float 	top, bot, lef, rig;		
	int 	img_wid, img_hei;
}cam_t;
//BASIC PROJECTION MATRIX CALCULATIONS
void vf_project( float aov, float near, float far, mat_4df M){
	float scale = 1.0 / tan(aov * 0.5 * M_PI / 180);
	M[0][0] = scale;
	M[1][1] = scale;
	M[2][2] = -far / (far - near);
	M[3][2] = -far * near / (far - near);
	M[2][3] = -1;
	M[3][3] = 0; 
}
//FRUSTUM BASED MATRIX CALCULATIONS
void vf_frustum(float b, float t, float l, float r, float n, float f, mat_4df M){
    M[0][0] = 2 * n / (float)(r - l);
    M[0][1] = 0;
    M[0][2] = 0;
    M[0][3] = 0;

    M[1][0] = 0;
    M[1][1] = 2 * n / (float)(t - b);
    M[1][2] = 0;
    M[1][3] = 0;

    M[2][0] = (r + l) / (float)(r - l);
    M[2][1] = (t + b) / (float)(t - b);
    M[2][2] = -(f + n) / (float)(f - n);
    M[2][3] = -1;

    M[3][0] = 0;
    M[3][1] = 0;
    M[3][2] = -2 * f * n / (float)(f - n);
    M[3][3] = 0;
}
//GLORTHO BASED CALCULATIONS
void vf_ortho(float b, float t, float l, float r, float n, float f, mat_4df M){
    M[0][0] = 2 / (r - l);
    M[0][1] = 0;
    M[0][2] = 0;
    M[0][3] = 0;

    M[1][0] = 0;
    M[1][1] = 2 / (t - b);
    M[1][2] = 0;
    M[1][3] = 0;

    M[2][0] = 0;
    M[2][1] = 0;
    M[2][2] = -2 / (f - n);
    M[2][3] = 0;

    M[3][0] = -(r + l) / (r - l);
    M[3][1] = -(t + b) / (t - b);
    M[3][2] = -(f + n) / (f - n);
    M[3][3] = 1;
}
//new camera
int new_camera(cam_t** cam, int W, int H, flag mode, vec_3df pos){
	cam_t* c = (cam_t*)malloc(sizeof *c);
	nptr(c);
	
	mat_4df idn = MTX_IDN;
	float b, t, l, r, iar;
	
	memcpy(&c->cam, idn, sizeof(idn));
	memcpy(&c->mpr, idn, sizeof(idn));	
	
	c->cam[3][1] = pos[0]; 
	c->cam[3][2] = pos[1]; 
	c->cam[3][3] = pos[2]; 	
	
	c->ncp = 1;
	c->fcp = 100;
	c->aov = 90;
		
	c->img_wid = W;
	c->img_hei = H;
	
	iar = W / (float)H;
		
	float scale = tan(c->aov * 0.5 * M_PI / 180.0) * (float)c->ncp;
	r = iar * scale;
	l = -r;
	t = scale;
	b = -t;
		
	if(mode & O_PROJT){
		vf_project(c->aov, c->ncp, c->fcp, c->mpr);
	}
	/*if(mode & O_FRUST){					
		vf_frustum(b, t, l, r, c->ncp, c->fcp, c->mpr);		
	}
	if(mode & O_ORTHO){		
		vf_ortho(b, t, l, r, c->ncp, c->fcp, c->mpr);
	}*/
	
	*cam = c;
}
void wvtx_to_scrc(cam_t* cam, vec_4df vtx, vec_2df rs){
	vec_3df rr;
	vec_mat_4df_dot(cam->cam, vtx, rr);
	vec_mat_4df_dot(cam->mpr, rr, rs);
	rs[0] = min(cam->img_wid - 1, (int)((rs[0] + 1) * 0.5 * cam->img_wid));
    rs[1] = min(cam->img_hei - 1, (int)((1 - (rs[1] + 1) * 0.5) * cam->img_hei));
}
//projection and face-cam intreactions
void wrld_to_scrc(cam_t* cam, face_t* face){
	norp(cam, face);
	vec_3df fwp = {face->bc->x, face->bc->y, face->bc->z};
	vec_3df cwp = {cam->cam[3][0], cam->cam[3][1], cam->cam[3][2]};
	/*type vertex to vec_4df cast*/
	vec_4df pw0, pw1, pw2;	//world coordinates
	vec_4df pc0, pc1, pc2;	//camera coordinates
	vec_3df rs0, rs1, rs2;	//raster coordinates
	/*copy point values to temp*/
	set_vec_4df(pw0, face->v0->sv->x, face->v0->sv->y, face->v0->sv->z, 1.0);
	set_vec_4df(pw1, face->v1->sv->x, face->v1->sv->y, face->v1->sv->z, 1.0);
	set_vec_4df(pw2, face->v2->sv->x, face->v2->sv->y, face->v2->sv->z, 1.0);
	/*transform world points to camera coordinates*/
	vec_mat_4df_dot(cam->cam, pw0, pc0);
	vec_mat_4df_dot(cam->cam, pw1, pc1);
	vec_mat_4df_dot(cam->cam, pw2, pc2);
	/*update face data*/
	set_p3df(face->v0->cc, pc0[0], pc0[1], pc0[2]);
	set_p3df(face->v1->cc, pc1[0], pc1[1], pc1[2]);
	set_p3df(face->v2->cc, pc2[0], pc2[1], pc2[2]);
	/*face to cam vector*/
	face_get_ftc(fwp,cwp,face->fc);
	/*transform camrea projected to matrix projection*/
	vec_mat_4df_dot(cam->mpr, pc0, rs0);
	vec_mat_4df_dot(cam->mpr, pc1, rs1);
	vec_mat_4df_dot(cam->mpr, pc2, rs2);
	/*convert to raster space*/	
	rs0[0] = min(cam->img_wid - 1, (int)((rs0[0] + 1) * 0.5 * cam->img_wid));
    rs0[1] = min(cam->img_hei - 1, (int)((1 - (rs0[1] + 1) * 0.5) * cam->img_hei));
    rs1[0] = min(cam->img_wid - 1, (int)((rs1[0] + 1) * 0.5 * cam->img_wid));
    rs1[1] = min(cam->img_hei - 1, (int)((1 - (rs1[1] + 1) * 0.5) * cam->img_hei));   
    rs2[0] = min(cam->img_wid - 1, (int)((rs2[0] + 1) * 0.5 * cam->img_wid));
    rs2[1] = min(cam->img_hei - 1, (int)((1 - (rs2[1] + 1) * 0.5) * cam->img_hei));
	/*update face data*/
	set_p2df(face->v0->pv, rs0[0], rs0[1]);	
	set_p2df(face->v1->pv, rs1[0], rs1[1]);
	set_p2df(face->v2->pv, rs2[0], rs2[1]);
	/*set z coordinate*/
	face->v0->z = z_inverse(rs0[2]);
	face->v1->z = z_inverse(rs1[2]);
	face->v2->z = z_inverse(rs2[2]);
}
#endif
