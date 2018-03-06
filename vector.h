#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <string.h>
#include <math.h>
#include <stdint.h>
#include "util/comptr.h"

#define MTX_IDN {							\
	{1.0000, 0.0000, 0.0000, 0.0000},		\
	{0.0000, 1.0000, 0.0000, 0.0000},		\
	{0.0000, 0.0000, 1.0000, 0.0000},		\
	{0.0000, 0.0000, 0.0000, 1.0000}		\
};	

#ifndef __SSE2__
#error "SSE2 not present"
#endif

#include <emmintrin.h>

#define min(a, b) 					\
	({ __typeof__ (a) _a = (a); 	\
	__typeof__ (b) _b = (b);	 	\
	_a < _b ? _a : _b; })	

#define max(a, b) 					\
	({ __typeof__ (a) _a = (a); 	\
	__typeof__ (b) _b = (b);	 	\
	_a > _b ? _a : _b; })	

#define p_shuffle(x, y, z, t) 			\
	((x) | ((y)<<2) | ((z)<<4) | ((t)<<6))

//util vector models
typedef int		vec_2di[2];
typedef int		vec_3di[3];
typedef int 	vec_4di[4];

typedef float	vec_2df[2];
typedef float	vec_3df[3];
typedef float	vec_4df[4];

typedef float 	mat_4df[4][4];

typedef uint8_t		flag;
//FUNCTION DEFINITIONS
//****************************//
/*********VECTOR STUFF*********/
//****************************//
int set_vec_2di(vec_2di pt, int x, int y){
	pt[0] = x;
	pt[1] = y;
	return 0;
}

int set_vec_3di(vec_3di pt, int x, int y, int z){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
	return 0;
}

void set_vec_4di(vec_4di pt, int x, int y, int z, int t){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
	pt[3] = t;
}
/*FLOAT VECTOR FROM FLOATS*/
void set_vec_2df(vec_2df pt, float x, float y){
	pt[0] = x;
	pt[1] = y;
}

void set_vec_3df(vec_3df pt, float x, float y, float z){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
}

void set_vec_4df(vec_4df pt, float x, float y, float z, float t){
	pt[0] = x;
	pt[1] = y;
	pt[2] = z;
	pt[3] = t;
}
/*COPY CONSTRUCTOR*/
void vec_2di_cp(vec_2di src, vec_2di dst){
	dst[0] = src[0];
	dst[1] = src[1];
}

void vec_3di_cp(vec_3di src, vec_3di dst){
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void vec_4di_cp(vec_4di src, vec_4di dst){
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];
}

void vec_2df_cp(vec_2df src, vec_2df dst){
	dst[0] = src[0];
	dst[1] = src[1];
}

void vec_3df_cp(vec_3df src, vec_3df dst){
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

void vec_4df_cp(vec_4df src, vec_4df dst){
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	dst[3] = src[3];
}
//-----------------------------------------
float min_3f(float a, float b, float c){
	float tmp = (a < b)	 ? a : b;
	return (c < tmp)	 ? c : tmp;
}

float max_3f(float a, float b, float c){
	float tmp = (a > b)	 ? a : b;
	return (c > tmp)	 ? c : tmp;
}
//-----------------------------------------
int min_3i(int a, int b, int c){
	int tmp = (a < b)	 ? a : b;
	return (c < tmp)	 ? c : tmp;
}

int max_3i(int a, int b, int c){
	int tmp = (a > b)	 ? a : b;
	return (c > tmp)	 ? c : tmp;
}
//-----------------------------------------
//****************************//
/*********VECTOR STUFF*********/
//****************************//
void add_3df(vec_3df a, vec_3df b, vec_3df r){
	r[0] = a[0] + b[0];
	r[1] = a[1] + b[1];
	r[2] = a[2] + b[2];
}

void sub_3df(vec_3df a, vec_3df b, vec_3df r){
	r[0] = a[0] - b[0];
	r[1] = a[1] - b[1];
	r[2] = a[2] - b[2];
}

float dot_3df(vec_3df a, vec_3df b){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

void crs_3df(vec_3df a, vec_3df b, vec_3df r){
	r[0] = a[1]*b[2]-a[2]*b[1];
	r[1] = a[2]*b[0]-a[0]*b[2];
	r[2] = a[0]*b[1]-a[1]*b[0];
}

void smul_3df(vec_3df a, float s){
	a[0] *= s; a[1] *= s; a[2] *= s;	
}

void sdiv_3df(vec_3df a, float s){ 
	a[0] /= s; a[1] /= s; a[2] /= s;
}

float len_3df(vec_3df a){
	return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}

void nrm_3df(vec_3df a){
	float w  = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	a[0] = a[0]/w, a[1] = a[1]/w, a[2] = a[2]/w;
}

void inv_3df(vec_3df a){
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}

void abs_3df(vec_3df a){
	a[0] = fabs(a[0]);
	a[1] = fabs(a[1]);
	a[2] = fabs(a[2]);
}

void ctr_3df(vec_3df a, float min, float max){
	a[0] = max(min, a[0]);
	a[1] = max(min, a[1]);
	a[2] = max(min, a[2]);
		
	a[0] = min(max, a[0]);
	a[1] = min(max, a[1]);
	a[2] = min(max, a[2]);
}

void out_3df(vec_3df v){
	//*printf("%f %f %f\n",v[0], v[1], v[2]);
}

//-----------------------------------------
//****************************//
/*********MATRIX STUFF*********/
//****************************//
/*4x4 MATMUL*/
void mat_4df_mul(mat_4df a, mat_4df b, mat_4df r){		
	__m128 r0 = _mm_loadu_ps(b[0]);
	__m128 r1 = _mm_loadu_ps(b[1]);
	__m128 r2 = _mm_loadu_ps(b[2]);
	__m128 r3 = _mm_loadu_ps(b[3]);
	
	__m128 dr0 = _mm_mul_ps(r0, _mm_set1_ps(a[0][0]));
	dr0 = _mm_add_ps(dr0, _mm_mul_ps(r1, _mm_set1_ps(a[0][1])));
    dr0 = _mm_add_ps(dr0, _mm_mul_ps(r2, _mm_set1_ps(a[0][2])));
	dr0 = _mm_add_ps(dr0, _mm_mul_ps(r3, _mm_set1_ps(a[0][3])));
	
	__m128 dr1 = _mm_mul_ps(dr0, _mm_set1_ps(a[1][0]));
    dr1 = _mm_add_ps(dr1, _mm_mul_ps(r1, _mm_set1_ps(a[1][1])));
    dr1 = _mm_add_ps(dr1, _mm_mul_ps(r2, _mm_set1_ps(a[1][2])));
    dr1 = _mm_add_ps(dr1, _mm_mul_ps(r3, _mm_set1_ps(a[1][3])));

    __m128 dr2 = _mm_mul_ps(dr0, _mm_set1_ps(a[2][0]));
    dr2 = _mm_add_ps(dr2, _mm_mul_ps(r1, _mm_set1_ps(a[2][1])));
    dr2 = _mm_add_ps(dr2, _mm_mul_ps(r2, _mm_set1_ps(a[2][2])));
    dr2 = _mm_add_ps(dr2, _mm_mul_ps(r3, _mm_set1_ps(a[2][3])));

    __m128 dr3 = _mm_mul_ps(dr0, _mm_set1_ps(a[3][0]));
    dr3 = _mm_add_ps(dr3, _mm_mul_ps(r1, _mm_set1_ps(a[3][1])));
    dr3 = _mm_add_ps(dr3, _mm_mul_ps(r2, _mm_set1_ps(a[3][2])));
    dr3 = _mm_add_ps(dr3, _mm_mul_ps(r3, _mm_set1_ps(a[3][3])));

    _mm_storeu_ps(r[0], dr0);
    _mm_storeu_ps(r[1], dr1);
    _mm_storeu_ps(r[2], dr2);
	_mm_storeu_ps(r[3], dr3);
}
/*4xN VEC MAT MUL*/
void vec_mat_4df_mul(mat_4df m, vec_4df v, vec_4df r){	
	__m128 r0 = _mm_loadu_ps(m[0]);
	__m128 r1 = _mm_loadu_ps(m[1]);
	__m128 r2 = _mm_loadu_ps(m[2]);
	__m128 r3 = _mm_loadu_ps(m[3]);
	
	__m128 vr = _mm_loadu_ps(v);	
	
	__m128 x = _mm_shuffle_ps(vr, vr, p_shuffle(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(vr, vr, p_shuffle(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(vr, vr, p_shuffle(2, 2, 2, 2));
	__m128 t = _mm_shuffle_ps(vr, vr, p_shuffle(3, 3, 3, 3));
	
	__m128 xr = _mm_mul_ps(x, r0);
	__m128 yr = _mm_mul_ps(y, r1);
	__m128 zr = _mm_mul_ps(z, r2);
	__m128 tr = _mm_mul_ps(t, r3);
	
	vr = _mm_add_ps(xr,yr);
	vr = _mm_add_ps(vr,zr);
	vr = _mm_add_ps(vr,tr);
	
	_mm_storeu_ps(r, vr);
}
/*4x4 TRANSPOSE*/
void mat_4df_transpose(mat_4df m){
		
	__m128 r0 = _mm_loadu_ps(m[0]);
	__m128 r1 = _mm_loadu_ps(m[1]);
	__m128 r2 = _mm_loadu_ps(m[2]);
	__m128 r3 = _mm_loadu_ps(m[3]);
	
	_MM_TRANSPOSE4_PS(r0, r1, r2, r3);
	
	_mm_storeu_ps(m[0], r0);
	_mm_storeu_ps(m[1], r1);
	_mm_storeu_ps(m[2], r2);
	_mm_storeu_ps(m[3], r3);
}
/*4x4 INVERSE*/
void mat4df_invert(mat_4df a, mat_4df r){
	
    __m128 m0, m1, m2, m3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp;
    
    __m128 r0 = _mm_loadu_ps(a[0]);
	__m128 r1 = _mm_loadu_ps(a[1]);
	__m128 r2 = _mm_loadu_ps(a[2]);
	__m128 r3 = _mm_loadu_ps(a[3]);
	
   	tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (__m64*)(*(a+0)+0)), (__m64*)(*(a+1)+0));
    row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(*(a+2)+0)), (__m64*)(*(a+3)+0));

    row0 = _mm_shuffle_ps(tmp, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp, 0xDD);

    tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (__m64*)(*(a+0)+2)), (__m64*)(*(a+1)+2));
    row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(*(a+2)+2)), (__m64*)(*(a+3)+2));

    row2 = _mm_shuffle_ps(tmp, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp, 0xDD);

    tmp = _mm_mul_ps(row2, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

    m0 = _mm_mul_ps(row1, tmp);
    m1 = _mm_mul_ps(row0, tmp);

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), m0);
    m1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), m1);
    m1 = _mm_shuffle_ps(m1, m1, 0x4E);

    tmp = _mm_mul_ps(row1, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

    m0 = _mm_add_ps(_mm_mul_ps(row3, tmp), m0);
    m3 = _mm_mul_ps(row0, tmp);

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m0 = _mm_sub_ps(m0, _mm_mul_ps(row3, tmp));
    m3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), m3);
    m3 = _mm_shuffle_ps(m3, m3, 0x4E);

    tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);

    m0 = _mm_add_ps(_mm_mul_ps(row2, tmp), m0);
    m2 = _mm_mul_ps(row0, tmp);

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m0 = _mm_sub_ps(m0, _mm_mul_ps(row2, tmp));
    m2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), m2);
    m2 = _mm_shuffle_ps(m2, m2, 0x4E);

    tmp = _mm_mul_ps(row0, row1);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

    m2 = _mm_add_ps(_mm_mul_ps(row3, tmp), m2);
    m3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), m3);

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), m2);
    m3 = _mm_sub_ps(m3, _mm_mul_ps(row2, tmp));

    tmp = _mm_mul_ps(row0, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

    m1 = _mm_sub_ps(m1, _mm_mul_ps(row2, tmp));
    m2 = _mm_add_ps(_mm_mul_ps(row1, tmp), m2);

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m1 = _mm_add_ps(_mm_mul_ps(row2, tmp), m1);
    m2 = _mm_sub_ps(m2, _mm_mul_ps(row1, tmp));

    tmp = _mm_mul_ps(row0, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

    m1 = _mm_add_ps(_mm_mul_ps(row3, tmp), m1);
    m3 = _mm_sub_ps(m3, _mm_mul_ps(row1, tmp));

    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

    m1 = _mm_sub_ps(m1, _mm_mul_ps(row3, tmp));
    m3 = _mm_add_ps(_mm_mul_ps(row1, tmp), m3);

    det = _mm_mul_ps(row0, m0);
    det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

    tmp = _mm_rcp_ss(det);

    det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
    det = _mm_shuffle_ps(det, det, 0x00);

    m0 = _mm_mul_ps(det, m0);
    m1 = _mm_mul_ps(det, m1);
    m2 = _mm_mul_ps(det, m2);
    m3 = _mm_mul_ps(det, m3);
    
    _mm_storeu_ps(r[0],m0);
    _mm_storeu_ps(r[1],m1);
    _mm_storeu_ps(r[2],m2);
    _mm_storeu_ps(r[3],m3);
}

void vec_mat_4df_dot(mat_4df M, vec_4df in, vec_3df out){
	out[0]   = in[0] * M[0][0] + in[1] * M[1][0] + in[2] * M[2][0] + 1 * M[3][0]; 
    out[1]   = in[0] * M[0][1] + in[1] * M[1][1] + in[2] * M[2][1] + 1 * M[3][1];
    out[2]   = in[0] * M[0][2] + in[1] * M[1][2] + in[2] * M[2][2] + 1 * M[3][2];
    
    float w  = in[0] * M[0][3] + in[1] * M[1][3] + in[2] * M[2][3] +  M[3][3];

    if (w != 1) {
        out[0] /= w;
        out[1] /= w;
        out[2] /= w; 
    }
}
#endif

