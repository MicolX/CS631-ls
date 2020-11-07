/*
 * This program implements the 'ls' command.
 * Author: Mingyao Xiong
 * Version: 1
 */

#include "ls.h"
#include "print.h"
#include "sort.h"

int
main(int argc, char **argv)
{
	int hasopt = 0;
	int start = 0;
	int arglen = 0;
	char op;
	FTSENT *ent;
	FTS *ftsp;
	options *opt; 
	struct stat *statp;

	if ((opt = (options *) malloc(sizeof(options))) == NULL) {
		err(EXIT_FAILURE, "malloc");
	}
	
	while ((op = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
		switch(op) {
			case 'A':
				opt->flag_A = 1;
				hasopt = 1;
				break;
			case 'a':
				opt->flag_a = 1;
				hasopt = 1;
				break;
			case 'c':
				opt->flag_c = 1;
				opt->flag_u = 0;
				hasopt = 1;
				break;
			case 'd':
				opt->flag_d = 1;
				opt->flag_R = 0;
				hasopt = 1;
				break;
			case 'F':
				opt->flag_F = 1;
				hasopt = 1;
				break;
			case 'f':
				opt->flag_f = 1;
				opt->flag_S = 0;
				opt->flag_r = 0;
				opt->flag_t = 0;
				hasopt = 1;
				break;
			case 'h':
				opt->flag_h = 1;
				opt->flag_k = 0;
				hasopt = 1;
				break;
			case 'i':
				opt->flag_i = 1;
				hasopt = 1;
				break;
			case 'k':
				opt->flag_k = 1;
				opt->flag_h = 0;
				hasopt = 1;
				break;
			case 'l':
				opt->flag_l = 1;
				hasopt = 1;
				break;
			case 'n':
				opt->flag_n = 1;
				hasopt = 1;
				break;
			case 'q':
				opt->flag_q = 1;
				opt->flag_w = 0;
				hasopt = 1;
				break;
			case 'R':
				if (opt->flag_d == 0) {
					opt->flag_R = 1;
				}
				hasopt = 1;
				break;
			case 'r':
				if (opt->flag_f == 0) {
					opt->flag_r = 1;
				}
				hasopt = 1;
				break;
			case 'S':
				if (opt->flag_f == 0) {
					opt->flag_S = 1;
				}
				opt->flag_t = 0;
				hasopt = 1;
				break;
			case 's':
				opt->flag_s = 1;
				hasopt = 1;
				break;
			case 't':
				if (opt->flag_f == 0) {
					opt->flag_t = 1;
				}
				opt->flag_S = 0;
				hasopt = 1;
				break;
			case 'u':
				opt->flag_u = 1;
				opt->flag_c = 0;
				hasopt = 1;
				break;
			case 'w':
				opt->flag_w = 1;
				opt->flag_q = 0;
				hasopt = 1;
				break;
			default:
				fprintf(stderr, "Invalid options\n");
				exit(EXIT_FAILURE);
		}
	}


	if ((hasopt == 0 && argc == 1) || (hasopt == 1 && argc == 2)) {
		start = -1;
	} else if (hasopt == 1 && argc > 2) {
		start = 2;
	} else if (hasopt == 0 && argc > 1) {
		start = 1;
	}
	
	if (start == -1) {
		arglen = 1;
	} else {
		arglen = argc - start;
	}

	char *operands[arglen+1];
	if (start == -1) {
		operands[0] = ".";
	} else {
		for (int i = start; i < start + arglen; i++) {
			operands[i - start] = argv[i];
		}
	}
	operands[arglen] = NULL;

	if ((hasopt == 0 && argc > 2) || (hasopt == 1 && argc > 3)) {
		qsort((void *)operands, sizeof(operands), sizeof(operands[0]), compar);
		if (errno) {
			err(EXIT_FAILURE, "sort");
		}
	}

	if (isatty(fileno(stdout)) == 1) {
		opt->flag_w = 0;
		opt->flag_q = 1;
	} else {
		opt->flag_w = 1;
		opt->flag_q = 0;
	}

	if (opt->flag_t) {
		if (opt->flag_c) {
			if (opt->flag_r) {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, byctime_rev);
			} else {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, byctime);
			}
		} else if (opt->flag_u) {
			if (opt->flag_r) {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, byatime_rev);
			} else {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, byatime);
			}
		} else {
			if (opt->flag_r) {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, bymtime_rev);
			} else {
				ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, bymtime);
			}
		}
	} else if (opt->flag_S) {
		if (opt->flag_r) {
			ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, bysize_rev);
		} else {
			ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, bysize);
		}
	} else {
		if (opt->flag_r) {
			ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, alphb_rev);
		} else {
			ftsp = fts_open(operands, FTS_PHYSICAL|FTS_SEEDOT, alphb);
		}
	}

	if (errno != 0) {
		fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	if (getuid() == 0) {
		opt->flag_A = 1;
	}

	while ((ent = fts_read(ftsp))) {
		if (errno) {
			fprintf(stderr, "Failed to read file: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (ent->fts_info == FTS_DP) {
			continue;
		}

		if (opt->flag_d && ent->fts_level > 0) {
			continue;
		}

		if (ent->fts_name[0] == '.' && !opt->flag_a && ent->fts_level > 0) {
			if (!opt->flag_A) {
				continue;
			}
			if (strncmp(ent->fts_name, ".", strlen(ent->fts_name)) == 0 || strncmp(ent->fts_name, "..", strlen(ent->fts_name)) == 0) {
				continue;
			}
		}

		if (ent->fts_info == FTS_D && (ent->fts_level == 0 || opt->flag_R)) {
			if (arglen > 1 || opt->flag_R) {
				printf("%s:\n", basename(ent->fts_path));
			}

			if (opt->flag_l) {
		 		printtotal(ent->fts_path);
			}
			continue;
		}	

		statp = ent->fts_statp;

		if (ent->fts_level < 2 || opt->flag_R) {
			if (opt->flag_i) {
				printf("%ju ", (uintmax_t)statp->st_ino);
			}
	
			if (opt->flag_l || opt->flag_n) {
				lprint(statp, opt);
			}

			if (opt->flag_q) {	
		 		printname(ent->fts_name);
			} else if (opt->flag_w) {
				printf("%s", ent->fts_name);
			}

			if (opt->flag_F) {
				if (S_ISDIR(statp->st_mode)) {
					printf("/");
				} else if (S_ISFIFO(statp->st_mode)) {
					printf("|");
				} else if (S_ISLNK(statp->st_mode)) {
					printf("@");
				} else if (S_ISWHT(statp->st_mode)) {	
					printf("%%");
				} else if (S_ISSOCK(statp->st_mode)) {
					printf("=");
				} else if ((statp->st_mode & S_IXUSR) || (statp->st_mode & S_IXGRP) || (statp->st_mode & S_IXOTH)) {
					printf("*");
				}  
			}

			if (S_ISLNK(statp->st_mode) && opt->flag_l && !opt->flag_d) {
				char buf[PATH_MAX];
				ssize_t len;
				if ((len = readlink(ent->fts_name, buf, sizeof(buf))) == -1) {
					fprintf(stderr, "Failed to fetch the source of a link '%s': %s\n", ent->fts_name, strerror(errno));
					exit(EXIT_FAILURE);
				}
				buf[len] = '\0';
				printf(" -> %s", buf);
			}

			printf("\n");
		}
	}
	
	free(opt);
	(void)fts_close(ftsp);
	return EXIT_SUCCESS;
}

