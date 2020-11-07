#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fts.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int (alphb) (const FTSENT **, const FTSENT **);
int (alphb_rev) (const FTSENT **, const FTSENT **);

int (compar) (const FTSENT **, const FTSENT **);
int (compar_rev) (const FTSENT **, const FTSENT **);

int (bymtime) (const FTSENT **, const FTSENT **);
int (bymtime_rev) (const FTSENT **, const FTSENT **);

int (byatime) (const FTSENT **, const FTSENT **);
int (byatime_rev) (const FTSENT **, const FTSENT **);

int (byctime) (const FTSENT **, const FTSENT **);
int (byctime_rev) (const FTSENT **, const FTSENT **);

int (bysize) (const FTSENT **, const FTSENT **);
int (bysize_rev) (const FTSENT **, const FTSENT **);

