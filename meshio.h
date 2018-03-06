#ifndef MESHIO_H
#define MESHIO_H

#include "mesh.h"
#include "util/dll.h"
#include "util/comptr.h"

#define CHR_SIZ		sizeof(char)

#define O_RDONLY		"r+"
#define O_WRONLY		"w+"

#define STR_END		'\0'
#define NUL_PTR		1
//parse raw by rows
int raw_prs(FILE* fp, p3df* pt){
	int bc = 0;
	bc = fscanf(fp, "%f", &pt->x);
	bc += bc;
	bc = fscanf(fp, "%f", &pt->y);
	bc += bc;
	bc = fscanf(fp, "%f", &pt->z);
	bc += bc;
	return bc;
}
//make 3d space points from raw row
int rawto_p3df(char* fname, dl_list* list){
	FILE* fp = fopen(fname, O_RDONLY);
	nptr(fp);
	fprintf(stderr, "OK: %s\n", fname);
	dl_list p3df_list = NULL;
	p3df* pt;
	while(1){
		new_p3df(&pt);
		//parse points		
		if(raw_prs(fp, pt) > 0){
			dll_insertt(&p3df_list, pt);
		}else{
			free(pt);
			break;
		}
	}
	*list = p3df_list;
	return 0;
}
//make a vertex and face list, from a list of ordinated points
void mklist_vfrawf(char* fname, dl_list* facl, dl_list* vtxl){
	nptr(fname);
	dl_list p3df_list = NULL;
	dl_list face_list = NULL;
	dl_list vrtx_list = NULL;
	//make point list from raw file rows
	rawto_p3df(fname, &p3df_list);
	//delete repeated points
	
	//iterate over points, and make face, vrtx lists
	p3df* pt0,* pt1,* pt2;
	vrtx_t* v0,* v1,* v2;
	face_t* ftmp;
	int i=0, j=0, k;
	list_it it = p3df_list;
	while(hasNext(it)){
		//retrieve points values by ref
		pt0 = it->data; it = it->next;	
		pt1 = it->data; it = it->next;
		pt2 = it->data; it = it->next;
		//assign points values to vertex coords
		new_vrtxi(&v0,i);   v0->sv = pt0;
		new_vrtxi(&v1,i+1); v1->sv = pt1;
		new_vrtxi(&v2,i+2); v2->sv = pt2;
		i+=3; 
		//also make vertex list
		dll_inserth(&vrtx_list, v0);
		dll_inserth(&vrtx_list, v1);
		dll_inserth(&vrtx_list, v2);	
		//eliminate repeated vertexes
		list_it a = vrtx_list;
		a = a->next->next->next;
		while(hasNext(a)){
			if(vrtx_eql(v0, a->data) > 0){
				v0 = a->data;
			}
			if(vrtx_eql(v1, a->data) > 0){
				v1 = a->data;
			}
			if(vrtx_eql(v2, a->data) > 0){
				v2 = a->data;
			}	
			a = a->next;
		}
		//assign vertex value to face
		new_facei(&ftmp,j);
		ftmp->v0 = v0; 
		ftmp->v1 = v1; 
		ftmp->v2 = v2; 
		//calculate face centroid
		face_get_cnt(ftmp);
		//calculate face normal
		face_get_nrm(ftmp);
		//insert in face list
		dll_insertt(&face_list, ftmp);
		j++;
	}
	*facl = face_list;
	*vtxl = vrtx_list;
}
//recalculate face attributes
void calc_fattr(dl_list face_list){
	list_it it = face_list;
	while(hasNext(it)){
		//calculate face centroid
		face_get_cnt(it->data);
		//calculate face normal
		face_get_nrm(it->data);
		it = it->next;
	}
}
//recalculate vertex attributes
void calc_vattr(dl_list* vlf_list, dl_list vrtx_list){
	int len = dll_length(vrtx_list);
	int i;
	list_it ft;
	list_it it = vrtx_list;
	//calculate vertex normal
	while(hasNext(it)){
		for(i=0;i<len;i++){
			vrtx_get_nrm(vlf_list[i], it->data);
			it = it->next;
		}		
	}
}
//make vertex list format from face and vertex list
void mklist_vlf(dl_list* vlf_list, dl_list face_list, dl_list vrtx_list){
	list_it vit = vrtx_list;
	list_it fit = face_list;
	int i;
	//make vlf
	while(hasNext(fit)){	
		vit = vrtx_list;
		i = 0;
		while(hasNext(vit)){
			if(face_cont_vrtx(fit->data, vit->data) > 0){
				dll_insertt(&vlf_list[i], fit->data);	
			}
			vit = vit->next;
			i++;
		}	
		fit = fit->next;
	}
	calc_vattr(vlf_list, vrtx_list);
}
#endif

