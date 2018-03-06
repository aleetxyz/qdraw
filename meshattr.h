#ifndef MESHATTR_H
#define MESHATTR_H

#include <stdlib.h>
#include <string.h>

#include "util/comptr.h"
#include "util/dll.h"
#include "transform.h"
#include "shading.h"
#include "mesh.h"

#define MSH_MTL		"MSH_MTL"
#define C_PROJT		"C_PROJT"
#define C_FRUST		"C_FRUST"
#define C_ORTHO		"C_ORTHO"
#define L_AMBNT		"L_AMBNT"
#define L_POINT		"L_POINT"
#define L_SPOTL		"L_SPOTL"
#define S_FLATS		"S_FLATS"
#define S_GOURA		"S_GOURA"
#define S_PHONG		"S_PHONG"
#define MSH_TRF		"MSH_TRF"

#define MPT_DLM		'\\'
#define P_TYPEL		3
#define MPT_ARG		5
#define MPT_AGS		7

char** str_split(char* a_str, char a_delim, int* argc)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp){
        if (a_delim == *tmp){
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str+strlen(a_str)-1);
    count++;

    result = (char**)malloc(sizeof(char*)*count);

    if (result){
        size_t idx=0;
        char* token = strtok(a_str,delim);

        while (token){
            *(result+idx++) = strdup(token);
            token = strtok(0,delim);
        }
        *(result+idx) = 0;
    }
    *argc = count-2;
    return result;
}

int mesh_getattr(char* fname, material** srf_mat, flag L_OPT, light** lig_src, dl_list* tl){
	nptr(fname);
	FILE* fp = fopen(fname,"r+");
	nptr(fp);
	//get file size
	char* buff;
	fseek(fp, 0L, SEEK_END);
	int fs = ftell(fp);
	rewind(fp);
	//allocate buffer w/fsize
	buff = (char*)calloc(fs,sizeof *buff);
	fread(buff, sizeof *buff, fs, fp);
	//mk structures
	//material
	material* m; new_material(&m, 0.0, 0.0, 0.0, 0);
	//light source
	light* l; new_light(&l);
	//transformation list
	dl_list trnf_list = NULL;
	transform* t;
	//start "parsing"
	float a, b, c, d;
	int i, j, k;
	char** group;
	char** propt;
	
	char type[P_TYPEL];
	
	group = str_split(buff,MPT_DLM,&k);
		
	if(strncmp(group[0],MSH_MTL,MPT_AGS) == 0){
		propt = str_split(group[0],'\n',&j);
		//set material properties ka, kd, ks, n
		sscanf(propt[1], "%s %f %f %f %d\n", type, &m->ka, &m->kd, &m->ks, &m->shns);
		//set surface color
		sscanf(propt[2], "%s %f %f %f\n", type, &m->color->r, &m->color->g, &m->color->b);
	}
	if((strncmp(group[1],L_AMBNT,MPT_AGS) == 0) && (L_OPT & O_AMBTL)){
		propt = str_split(group[1],'\n',&j);
		new_ambient(l);	
		sscanf(propt[1], "%s %f %f %f\n", type, &l->ambient->pos[0], &l->ambient->pos[1], &l->ambient->pos[2]);
		sscanf(propt[2], "%s %f %f %f\n", type, &l->lint->r, &l->lint->g, &l->lint->b);
	}
	if((strncmp(group[2],L_POINT,MPT_AGS) == 0) && (L_OPT & O_POINT)){
		propt = str_split(group[2],'\n',&j);
		new_point(l);
		sscanf(propt[1], "%s %f %f %f\n", type, &l->point->pos[0], &l->point->pos[1], &l->point->pos[2]);
		sscanf(propt[2], "%s %f %f %f\n", type, &l->lint->r, &l->lint->g, &l->lint->b);
		sscanf(propt[3], "%s %f %f %f\n", type, &l->point->att[0], &l->point->att[1], &l->point->att[2]);
	}
	if((strncmp(group[3],L_SPOTL,MPT_AGS) == 0) && (L_OPT & O_SPOTL)){
		propt = str_split(group[3],'\n',&j);
		new_spot(l);
		sscanf(propt[1], "%s %f %f %f\n", type, &l->spot->pos[0], &l->spot->pos[1], &l->spot->pos[2]);
		sscanf(propt[2], "%s %f %f %f\n", type, &l->lint->r, &l->lint->g, &l->lint->b);
		sscanf(propt[3], "%s %f %f %f\n", type, &l->spot->att[0], &l->spot->att[1], &l->spot->att[2]);
		sscanf(propt[4], "%s %f %f %f\n", type, &l->spot->pnt[0], &l->spot->pnt[1], &l->spot->pnt[2]);
		sscanf(propt[5], "%s %f\n", type, &l->spot->cutoff);
	}
	if(strncmp(group[4],MSH_TRF,MPT_AGS) == 0){
		propt = str_split(group[4],'\n',&j);
		for(i=1;i<=j;i++){
			new_transform(&t);
			fflush(fp);
			k = sscanf(propt[i], "%c %f %f %f %f\n", &t->t, &t->x, &t->y, &t->z, &t->a);
			if(k > 0){
				dll_insertt(&trnf_list, t);
			}
		}
	}
	*tl = trnf_list;
	*srf_mat = m;
	*lig_src = l;
}

flag getFlag(char* str, flag *f){
	if(strncmp(str,L_AMBNT,MPT_AGS) == 0){
		*f |= 0x1;
	}
	if(strncmp(str,L_POINT,MPT_AGS) == 0){
		*f |= 0x2;
	}
	if(strncmp(str,L_SPOTL,MPT_AGS) == 0){
		*f |= 0x4;
	}
	if(strncmp(str,S_FLATS,MPT_AGS) == 0){
		*f |= 0x10;
	}
	if(strncmp(str,S_GOURA,MPT_AGS) == 0){
		*f |= 0x20;
	}
	if(strncmp(str,S_PHONG,MPT_AGS) == 0){
		*f |= 0x40;
	}
	if(strncmp(str,C_PROJT,MPT_AGS) == 0){
		*f |= 0x5;
	}
	if(strncmp(str,C_FRUST,MPT_AGS) == 0){
		*f |= 0x6;
	}
	if(strncmp(str,C_ORTHO,MPT_AGS) == 0){
		*f |= 0x7;
	}
	else{
		return -1;
	}	
	return 1;
}
#endif
