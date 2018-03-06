#include <stdio.h>

#include "shading.h"
#include "meshio.h"
#include "meshattr.h"
#include "camera.h"

#include "util/dll.h"
#include "buffer/raster.c"
#include "buffer/zbuffer.c"
#include "buffer/fbuffer.c"

int main(int argc, char** argv){
	int i, j, k, s; i = j = k = 0;
	//init ARGS
	/*if(argc < 9){
		printf("Quick n' dirty raw worker\nUsage: %s [raw_file_name] [mesh_attrib_file_name]\n[C_PROJT|C_FRUST|C_ORTHO]\n[L_AMBNT|L_POINT|L_SPOTL]\n[S_FLATS|S_GOURA|S_PHONG]\n[nsecs] [nframes] [img_width] [img_height]\n", argv[0]);
		exit(EXIT_FAILURE);
	}*/

	//parse args
	char* rfname = "ico.raw";//argv[1];
	char* pfname = "mesh.mpt";//argv[2]
	flag copt, lopt, sopt;
	
	//if(getFlag(argv[3],&lopt) < 0){
		copt |= O_ORTHO;
	//}
	//if(getFlag(argv[4],&lopt) < 0){
		lopt |= O_SPOTL;
	//}
	//if(getFlag(argv[5],&sopt) < 0){
		sopt |= O_PHONG;
	//}
	int nsecs = 60;//atoi(argv[6]);
	int fps = 24;//atoi(argv[7]);
	int IMG_WID = 1920;//atoi(argv[8]);
	int IMG_HEI = 1080;//atoi(argv[9]);
	
	//make face and vertex lists
	dl_list face_list = NULL;
	dl_list vrtx_list = NULL;
	mklist_vfrawf(rfname, &face_list, &vrtx_list);
	
	//make vertex list format
	int len = dll_length(vrtx_list);
	dl_list* vlf_list = (dl_list*)malloc(sizeof(dl_list)*len);
	for(i=0;i<len;i++){
		vlf_list[i] = NULL;
	}	
	mklist_vlf(vlf_list, face_list, vrtx_list);
	
	//retrieve mesh properties
	dl_list trsf_list = NULL;
	material* m;
	light* l;
	mesh_getattr(pfname,&m,lopt,&l,&trsf_list);	
	
	//prepare image buffers
	new_fbuffer(IMG_WID, IMG_HEI);
	new_zbuffer(IMG_WID, IMG_HEI, CAM_FCP);
	
	//camera settings
	cam_t *camera;
	vec_3df pos = {0, -10, 3};
	new_camera(&camera, IMG_WID, IMG_HEI, copt, pos);

	//project every space point to camera-plane and get raster coords
	list_it it = face_list;
	while(hasNext(it)){
		wrld_to_scrc(camera, it->data);	
		it = it->next;
	}
			
	//apply face and vertex intensity (flat and gouraud)
	if (!(sopt |= O_PHONG))
		apply_meshi(face_list, l, m);
	
	//pre-raster stage
	color* rgb;
	new_color(&rgb);
	
	vec_3df p0, p1, p2, w;
	face_t* ftmp;
	int x, y; float z;
	
	//make list of transformations
	list_it ti = trsf_list;	
	i = 0;
	s = (nsecs*fps)/dll_length(trsf_list);
	
	while(hasNext(ti)){
		for(j=0;j<s;j++){
			//rewind pointer
			it = face_list;
			//transform all faces
			while(hasNext(it)){
				//read transform list, transform the face
				mk_trnf_f(it->data, ti->data, s);
				//project onto camera plane
				wrld_to_scrc(camera, it->data);	
				//get next face
				it = it->next;
			}
			//rewind pointer
			it = face_list;
			vec_3df pxi;
			//start rasterizaiton	
			while(hasNext(it)){
				ftmp = it->data;
				//recalculate normals
				calc_fattr(face_list);
				calc_vattr(vlf_list,vrtx_list);
				//get x, y screen coordinates
				set_vec_3df(p0, ftmp->v0->pv->x, ftmp->v0->pv->y, ftmp->v0->z);
				set_vec_3df(p1, ftmp->v1->pv->x, ftmp->v1->pv->y, ftmp->v1->z);
				set_vec_3df(p2, ftmp->v2->pv->x, ftmp->v2->pv->y, ftmp->v2->z);
				//get x, y, max, min values for bounding box
				int x0 = (int)min_3f(p0[0], p1[0], p2[0]);
				int y0 = (int)min_3f(p0[1], p1[1], p2[1]);		
				int x1 = (int)max_3f(p0[0], p1[0], p2[0]);	
				int y1 = (int)max_3f(p0[1], p1[1], p2[1]);
				//check clipping		
				if(x0 >= IMG_WID || x1 < 0 || y0 >= IMG_HEI || y1 < 0){
					it = it->next;
					continue;	
				}
				//screen bounds check
				x0 = max(0, x0); x1 = min(IMG_WID-1, x1);
				y0 = max(0, y0); y1 = min(IMG_HEI-1, y1);
				//rasterization per bounding box
				for(y = y0; y <= y1; y++){
					for(x = x0; x <= x1; x++){
						set_vec_3df(pxi, x + 0.5, y + 0.5, 0.0);
						w[0] = p_edgef(p1, p2, pxi);
						w[1] = p_edgef(p2, p0, pxi);
						w[2] = p_edgef(p0, p1, pxi);
						//check if point is inside the triangle
						if(w[0] >= 0 && w[1] >= 0 && w[2] >= 0){
							//get barycentric coordinates w/area
							sdiv_3df(w, p_edgef(p0, p1, p2));
							//interpolate Z
							z_interpolate(p0[2],p1[2],p2[2],w,&z);
							//depth test
							if(z_compare(x, y, z) > 0){
								z_setcoord(x, y, z);
								z_putpixel(x, y);
								//shading
								fshade(ftmp, l, m, w, sopt, rgb);
								f_putpixel(x, y, rgb);
							}
						}
					}
				}
				//next face
				it = it->next;
			}
			/*create files*/				
			zbuffer_ppm("rend/zbuf.ppm");
			//write frame buffer to file
			printf("TRANSFORM: %d, STEP: %d, INDEX: %d\n", i, j, k);
			char filename[16];
			sprintf(filename, "data/img%04d.ppm", k);
			fbuffer_ppm(filename);
			
			//next transform
			k++;
			rst_zbuffer(CAM_FCP);
			rst_fbuffer();
		}
		ti = ti->next;
		i++;
	}
	return 0;
}
