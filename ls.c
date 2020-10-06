/*
 * This program implements the 'ls' command.
 * Author: Mingyao Xiong
 * Version: 1
 */

#include "ls.h"


int
main(int argc, char **argv)
{
	DIR *dp;
	struct dirent *dirp;
	struct passwd *pswd;
	struct group *grp;
	int hasopt;
	char op;

	options *opt = (options *) calloc(1, sizeof(options));
	
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

	

	if ((dp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "Unable to open '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((dirp = readdir(dp)) != NULL) {
		struct stat sb;
		if (stat(dirp->d_name, &sb) == -1) {
			fprintf(stderr, "Failed to stat: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if ((pswd = getpwuid(sb.st_uid)) == NULL) {
			fprintf(stderr, "Unable to get info of this uid: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if ((grp = getgrgid(sb.st_gid)) == NULL) {
			fprintf(stderr, "Unable to get info of this gid: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		char p[10];
		printf("%s ", getPermission(p,sb));	
		printf("%d ", sb.st_nlink);
		((pswd->pw_name != NULL) & (strlen(pswd->pw_name) != 0)) ? printf("%s ", pswd->pw_name) : printf("%u ", sb.st_uid);
		((grp->gr_name != NULL) & (strlen(grp->gr_name) != 0)) ? printf("%s ", grp->gr_name) : printf("%u ", sb.st_gid);
		char *date = ctime(&sb.st_mtime);
		if (date[strlen(date)-1] == '\n') {
			date[strlen(date)-1] = 0;
		}

		printf("%s ", date);
		printf("%s\n", dirp->d_name);
	}

	(void)closedir(dp);
	return EXIT_SUCCESS;
}


char *
getPermission(char *perm, struct stat sb) 
{
	if (S_ISDIR(sb.st_mode)) {
		perm[0] = 'd';
	} else if (S_ISBLK(sb.st_mode)) {
		perm[0] = 'b';
	} else if (S_ISCHR(sb.st_mode)) {
		perm[0] = 'c';
	} else if (S_ISFIFO(sb.st_mode)) {
		perm[0] = 'P';
	} else if (S_ISLNK(sb.st_mode)) {
		perm[0] = 'l';
	} else if (S_ISSOCK(sb.st_mode)) {
		perm[0] = 's';
	} else {
		perm[0] = '-';
	}
	

	perm[1] = (sb.st_mode & S_IRUSR) ? 'r' : '-';
	perm[2] = (sb.st_mode & S_IWUSR) ? 'w' : '-';
	perm[3] = (sb.st_mode & S_IXUSR) ? 'x' : '-';
	perm[4] = (sb.st_mode & S_IRGRP) ? 'r' : '-';	
	perm[5] = (sb.st_mode & S_IWGRP) ? 'w' : '-';
	perm[6] = (sb.st_mode & S_IXGRP) ? 'x' : '-';
	perm[7] = (sb.st_mode & S_IROTH) ? 'r' : '-';
	perm[8] = (sb.st_mode & S_IWOTH) ? 'w' : '-';
	perm[9] = (sb.st_mode & S_IXOTH) ? 'x' : '-';
		

	return perm;
}