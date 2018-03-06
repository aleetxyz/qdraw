#include "dll.h"

/*initialize new node*/
int newNode(node** p, void* data){
	if(!vptr(p))
		return -1;
	*p = (node*)malloc(sizeof(node));
	(*p)->data = data;
	(*p)->prev = NULL;
	(*p)->next = NULL;
	return 0;
}
/*delete list*/
int dll_delete(dl_list list){
	node* temp = list;
	node* next = NULL;
	while(temp!=NULL){
		temp->data = NULL;
		next = temp->next;
		temp->prev = NULL;
		temp->next = NULL;
		free(temp);
		temp = next;
	}
	list = NULL;
	return 0;
}
/*calculate length*/
int dll_length(dl_list list){
	node* temp = list;
	int len = 0;
	while(temp!=NULL){
		len++;
		temp = temp->next;
	}
	return len;
}
/*insert item at head*/
int dll_inserth(dl_list* list, void* data){
	node* new_node;	
	newNode(&new_node, data);
	if(*(list) == NULL){
		*list = new_node;
		return 0;
	}	
	(*list)->prev = new_node;
	new_node->next = *list;
	*list = new_node;
	return 0;
}
/*insert item at tail*/
int dll_insertt(dl_list* list, void* data){
	node* temp = *list;
	node* new_node;
	newNode(&new_node, data);
	if(*(list) == NULL){
		*list = new_node;
		return 0;
	}
	while(temp->next != NULL) 
		temp = temp->next;
	temp->next = new_node;
	new_node->prev = temp;
	return 0;
}
/*remove head item*/
int dll_removeh(dl_list* list){
	node* temp = *list;
	*list = (*list)->next;
	free(temp);
	return 0;
}
/*remove tail item*/
int dll_removet(dl_list* list){
	node* temp = *list;
	while(temp->next != NULL) 
		temp = temp->next;
	temp->prev->next = NULL;
	free(temp);
	return 0;
}
/*print all items*/
/*list is origin list*/
/*print is funciton pointer user-specific where void* is the tad*/
int dll_print(dl_list* list, int(*print)(const void*)){
	node* temp = *list;
	int cw;
	while(temp != NULL){
		cw = print(temp->data);
		temp = temp->next;		
	}
	return 0;
}
