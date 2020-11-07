#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <errno.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printtotal(char *);
void printname(char *);
void lprint(struct stat *, options *);
