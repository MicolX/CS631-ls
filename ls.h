#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fts.h>
#include <grp.h>
#include <inttypes.h>
#include <math.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


typedef struct options_t {
	int flag_A;
	int flag_a;
	int flag_c;
	int flag_d;
	int flag_F;
	int flag_f;
	int flag_h;
	int flag_i;
	int flag_k;
	int flag_l;
	int flag_n;
	int flag_q;
	int flag_R;
	int flag_r;
	int flag_S;
	int flag_s;
	int flag_t;
	int flag_u;
	int flag_w;
} options;

