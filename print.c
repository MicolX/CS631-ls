#include "print.h"



void
printtotal(FTSENT **ent) {
	long long int total = 0;
	FTS *fp; 
	FTSENT *entp;
	char *path[2] = {(*ent)->fts_path, NULL};

	fp = fts_open(path, FTS_PHYSICAL|FTS_SEEDOT, NULL);

	if (errno) {
		fprintf(stderr, "Failed to read a directory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((entp = fts_read(fp))) {	
		if (entp->fts_level == 1) {	                   
			total += (long long)entp->fts_statp->st_blocks;																							                    }																																			                }

	if (errno) {
		fprintf(stderr, "Failed to traverse a directory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);																																			}	

	printf("total %lld\n", total);
}

void
printname(FTSENT **ent) {
	char *name = (*ent)->fts_name;
	int len = strlen(name);
	char new[len+1];

	for(int i = 0; i < len; i++) {
		char c = name[i];
		if (isprint(c) == 0) {
			new[i] = '?';
		} else {
			new[i] = c;
		}
	}

	new[len] = '\0';
	printf("%s", new);
}
