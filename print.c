#include "ls.h"
#include "print.h"


void
printtotal(char *dirpath) {
	long long int total = 0;
	FTS *fp; 
	FTSENT *entp;
	char *path[2] = {dirpath, NULL};

	fp = fts_open(path, FTS_PHYSICAL|FTS_SEEDOT, NULL);

	if (errno) {
		fprintf(stderr, "Failed to read a directory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((entp = fts_read(fp))) {	
		if (entp->fts_info == FTS_DP) {
			continue;
		}

		if (entp->fts_level == 1) {	                   
			total += (long long)entp->fts_statp->st_blocks;
		}
	}

	if (errno) {
		fprintf(stderr, "Failed to traverse a directory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);																																			}	

	printf("total %lld\n", total);
}

void
printname(char *name) {
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

void
lprint(struct stat *statp, options *opt) {
	char perm[11], timebuff[15], unit[] = {'B', 'K', 'M', 'G', 'T'};
	struct passwd *pw;
 	struct group *grp;
 	struct tm *timeinfo;
	
    strmode(statp->st_mode, perm);
	printf("%s ", perm);		 
	printf ("%-2ju ", (uintmax_t)statp->st_nlink);
	
	if (opt->flag_n) {	
		printf("%-5ju %-5ju ", (uintmax_t)statp->st_uid, (uintmax_t)statp->st_gid);
	} else {
		if ((pw = getpwuid(statp->st_uid)) == NULL) {
			fprintf(stderr, "Failed to get uid: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		if ((grp = getgrgid(statp->st_gid)) == NULL) {
			fprintf(stderr, "Failed to get gid: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		printf("%-5s %-5s ", pw->pw_name, grp->gr_name);
	}

	if (S_ISCHR(statp->st_mode) || S_ISBLK(statp->st_mode)) {
		printf("%d %d ", major(statp->st_rdev), minor(statp->st_rdev));
	} else {
		if (opt->flag_h) {
			double size = statp->st_size;
			int index = 0;
			while (size > 1024) {
				size /= 1024;
				index++;
			}
			
			if (index > 4) {
				fprintf(stderr, "A file is too large to print\n");
				exit(EXIT_FAILURE);
			}
			printf("%.1f%c ", size, unit[index]);
			
		} else if (opt->flag_s) {
			if (opt->flag_k) {
				printf("%.1fK", (long long int)statp->st_size / 1024.0);
			} else {
			//	printf("%lld ", (long long int)statp->st_blocks);
				printf("%d ", (int)ceil(statp->st_size / getblocksize()));
			}
		} else {
			printf("%5lld ", (long long int)statp->st_size);
		}
	}

	if (opt->flag_c) {
		timeinfo = localtime(&(statp->st_ctime));
	} else if (opt->flag_u) {
		timeinfo = localtime(&(statp->st_atime));
	} else {
		timeinfo = localtime(&(statp->st_mtime));
	}
	strftime(timebuff, 15, "%b %d %H:%M", timeinfo);
	printf("%13s ", timebuff);
}

int
getblocksize() {
	char *res;
	if ((res = getenv("BLOCKSIZE")) == NULL) {
		return 512;
	}

	if (res[0] == '-') {
		return 512;
	}

	return atoi(res);
}
