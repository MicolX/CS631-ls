#include <sys/types.h>
#include <sys/stat.h>

#include <fts.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int (compar) (const FTSENT **, const FTSENT **);
