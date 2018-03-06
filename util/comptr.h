
#define cptr(p,m)		if(p==NULL){			\
							fprintf(stderr,m);	\
							return -1;			\
						}

#define nptr(p)			if(p==NULL){					\
							fprintf(stderr,"NUL PTR");	\
							exit(EXIT_FAILURE);			\
						}

#define norp(pa,pb)		if(!pa || !pb){					\
							fprintf(stderr,"NUL PTR");	\
							exit(EXIT_FAILURE);			\
						}


#define nanp(pa,pb)		if(!pa && !pb){					\
							fprintf(stderr,"NUL PTR");	\
							exit(EXIT_FAILURE);			\
						}
