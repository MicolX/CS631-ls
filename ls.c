/*
 * This program implements the 'ls' command.
 * Author: Mingyao Xiong
 * Version: 1
 */

#include "ls.h"
#include "sort.h"

int
main(int argc, char **argv)
{
	struct passwd *pswd;
	int hasopt = 0;
	char op;
	FTSENT *ent;
	FTS *ftsp;
	options *opt = (options *) malloc(sizeof(options));
	int fts_options = FTS_PHYSICAL;
	int start = 0;
	int arglen = 0;
	
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
				opt->flag_R = 1;
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

	if (opt->flag_a) {
		fts_options = fts_options|FTS_SEEDOT;	
	}

	ftsp = fts_open(operands, fts_options, compar);
	if (errno != 0) {
		fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	if (getuid() == 0) {
		opt->flag_A = 1;
	}

	while ((ent = fts_read(ftsp))) {
		if (ent->fts_info == FTS_DP) {
			continue;
		}

		if (ent->fts_info == FTS_D && ent->fts_level == 0) {
			if (arglen > 1) {
				printf("%s:\n", ent->fts_name);
			}
			continue;
		}

		

		if (ent->fts_level < 2) {
			printf("%s\n", ent->fts_name);
		}
	}
	
	

	//if ((dp = opendir(argv[1])) == NULL) {
	//	fprintf(stderr, "Unable to open '%s': %s\n", argv[1], strerror(errno));
	//	exit(EXIT_FAILURE);
	//}

	//while ((dirp = readdir(dp)) != NULL) {
	//	struct stat sb;
	//	if (stat(dirp->d_name, &sb) == -1) {
	//		fprintf(stderr, "Failed to stat: %s\n", strerror(errno));
	//		exit(EXIT_FAILURE);
	//	}

	//	if ((pswd = getpwuid(sb.st_uid)) == NULL) {
	//		fprintf(stderr, "Unable to get info of this uid: %s\n", strerror(errno));
	//		exit(EXIT_FAILURE);
	//	}

	//	if ((grp = getgrgid(sb.st_gid)) == NULL) {
	//		fprintf(stderr, "Unable to get info of this gid: %s\n", strerror(errno));
	//		exit(EXIT_FAILURE);
	//	}
	//	char p[10];
	//	strmode(sb.st_mode, p);
	//	printf("%s ", p);	
	//	printf("%d ", sb.st_nlink);
	//	((pswd->pw_name != NULL) & (strlen(pswd->pw_name) != 0)) ? printf("%s ", pswd->pw_name) : printf("%u ", sb.st_uid);
	//	((grp->gr_name != NULL) & (strlen(grp->gr_name) != 0)) ? printf("%s ", grp->gr_name) : printf("%u ", sb.st_gid);
	//	char *date = ctime(&sb.st_mtime);
	//	if (date[strlen(date)-1] == '\n') {
	//		date[strlen(date)-1] = 0;
	//	}

	//	printf("%s ", date);
	//	printf("%s\n", dirp->d_name);
	//}

	(void)fts_close(ftsp);
	return EXIT_SUCCESS;
}

